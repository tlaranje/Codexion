/* Codexion Corrigido - Versão Monolítica Final */
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>  // Precisa para offsetof()

#define QUEUE_SIZE 4
#define QUEUE_MASK (QUEUE_SIZE - 1)
#define START_PATTERN 2
#define DEADLINE_START 0

/* FORWARD DECLARATIONS */
typedef struct s_coder t_coder;
typedef struct s_dongle t_dongle;
typedef struct s_config t_config;
typedef struct s_data t_data;

/* ENUMS */
enum e_enum { ENUM_SUCCESS, ENUM_FULL };
enum e_sim { SIM_RUN, SIM_STOP };
enum e_evt { EVT_DONGLE, EVT_COMPILE, EVT_DEBUG, EVT_REFACTOR, EVT_BURNOUT, EVT_MAX };


typedef struct s_dongle_queue
{
	t_coder *coders[QUEUE_SIZE];
	uint32_t head;
	uint32_t tail;
	pthread_mutex_t m;
} t_dongle_queue;

typedef enum e_enum (*t_enq_f)(t_dongle_queue *, t_coder *);

struct s_queue { 
    t_coder *coders[4]; 
    uint32_t head, tail; 
    pthread_mutex_t m; 
};

struct s_dongle { 
    pthread_mutex_t m; 
    const uint64_t *cool; 
    uint64_t cool_end; 
    bool used; 
    t_dongle_queue q; 
};

struct s_coder { 
    uint32_t id; 
    pthread_t th; 
    pthread_mutex_t m; 
    pthread_cond_t cv; 
    pthread_mutex_t *lm; 
    uint64_t dl; 
    struct timespec dts; 
    uint32_t done; 
    t_dongle *ld, *rd; 
    t_coder *ln, *rn; 
    t_enq_f enq; 
};

struct s_config { 
    uint32_t nc; 
    uint64_t ttb, ttc, ttd, ttr, nc2; 
    uint64_t dc; 
    t_enq_f sch; 
    uint64_t psm, sdm; 
    struct timespec sdts; 
};

struct s_data { 
    t_config *c; 
    t_coder *coders; 
    t_dongle *d; 
    pthread_t mt; 
};

static bool gsim = true;
static pthread_mutex_t gsim_m = PTHREAD_MUTEX_INITIALIZER;

bool sim_ctl(const bool *s) { 
    bool x; 
    pthread_mutex_lock(&gsim_m); 
    if (s) gsim = *s; 
    x = gsim; 
    pthread_mutex_unlock(&gsim_m); 
    return x; 
}

void stop_sim(void) { 
    const bool f = 0; 
    sim_ctl(&f); 
}

static inline bool is_run(void) { 
    return sim_ctl(NULL); 
}

static uint64_t ms(void) { 
    struct timeval tv; 
    gettimeofday(&tv, NULL); 
    return tv.tv_sec * 1000 + tv.tv_usec / 1000; 
}

static const char *msgs[5] = { 
    "has taken a dongle", "is compiling", "is debugging", "is refactoring", "burned out" 
};

void log_e(const t_coder *c, int e) { 
    if (e >= 0 && e < EVT_MAX) { 
        pthread_mutex_lock(c->lm); 
        if (e != EVT_BURNOUT || is_run()) 
            printf("%lu %d %s\n", ms(), c->id, msgs[e]); 
        pthread_mutex_unlock(c->lm); 
    } 
}

int qinit(t_dongle_queue *q) { 
    q->head = q->tail = 0; 
    return pthread_mutex_init(&q->m, NULL); 
}

int qdest(t_dongle_queue *q) { 
    return pthread_mutex_destroy(&q->m); 
}

enum e_enum fifo_enq(t_dongle_queue *q, t_coder *c) { 
    uint32_t nt; 
    pthread_mutex_lock(&q->m); 
    nt = (q->tail + 1) & QUEUE_MASK; 
    if (nt == q->head) 
        return pthread_mutex_unlock(&q->m), ENUM_FULL; 
    q->coders[q->tail] = c; 
    q->tail = nt; 
    return pthread_mutex_unlock(&q->m), ENUM_SUCCESS; 
}

void bubble(t_dongle_queue *q, uint32_t i) { 
    uint32_t p; 
    t_coder *tmp; 
    while (i != q->head) { 
        p = (i - 1 + QUEUE_SIZE) & QUEUE_MASK; 
        if (q->coders[i]->dl >= q->coders[p]->dl) break; 
        tmp = q->coders[i]; 
        q->coders[i] = q->coders[p]; 
        q->coders[p] = tmp; 
        i = p; 
    } 
}

enum e_enum edf_enq(t_dongle_queue *q, t_coder *c) { 
    uint32_t nt; 
    pthread_mutex_lock(&q->m); 
    nt = (q->tail + 1) & QUEUE_MASK; 
    if (nt == q->head) 
        return pthread_mutex_unlock(&q->m), ENUM_FULL; 
    q->coders[q->tail] = c; 
    q->tail = nt; 
    bubble(q, (nt - 1 + QUEUE_SIZE) & QUEUE_MASK); 
    return pthread_mutex_unlock(&q->m), ENUM_SUCCESS; 
}

t_coder *deq(t_dongle_queue *q) { 
    t_coder *c; 
    pthread_mutex_lock(&q->m); 
    if (q->head == q->tail) 
        return pthread_mutex_unlock(&q->m), NULL; 
    c = q->coders[q->head]; 
    q->head = (q->head + 1) & QUEUE_MASK; 
    return pthread_mutex_unlock(&q->m), c; 
}

void set_used(t_dongle *d) { 
    pthread_mutex_lock(&d->m); 
    d->used = 1; 
    pthread_mutex_unlock(&d->m); 
}

bool first(t_coder *me) { 
    t_dongle_queue *f = &me->ld->q, *s = &me->rd->q; 
    uint32_t h; 
    bool r; 
    if ((uintptr_t)f > (uintptr_t)s) { 
        f = &me->rd->q; 
        s = &me->ld->q; 
    } 
    pthread_mutex_lock(&f->m); 
    h = f->head; 
    r = h != f->tail && f->coders[h] == me; 
    pthread_mutex_unlock(&f->m); 
    if (!r) return 0; 
    pthread_mutex_lock(&s->m); 
    h = s->head; 
    r = h != s->tail && s->coders[h] == me; 
    pthread_mutex_unlock(&s->m); 
    return r; 
}

bool free_d(t_coder *me) { 
    t_dongle *f = me->ld, *s = me->rd; 
    bool a; 
    if ((uintptr_t)f > (uintptr_t)s) { 
        f = me->rd; 
        s = me->ld; 
    } 
    pthread_mutex_lock(&f->m); 
    pthread_mutex_lock(&s->m); 
    a = !f->used && !s->used; 
    pthread_mutex_unlock(&s->m); 
    pthread_mutex_unlock(&f->m); 
    return a; 
}

enum e_sim req_d(t_coder *c) { 
    t_dongle *l = c->ld, *r = c->rd; 
    pthread_mutex_lock(&c->m); 
    c->enq(&l->q, c); 
    c->enq(&r->q, c); 
    while (!first(c) || !free_d(c)) { 
        if (!is_run()) 
            return pthread_mutex_unlock(&c->m), SIM_STOP; 
        if (pthread_cond_timedwait(&c->cv, &c->m, &c->dts) == ETIMEDOUT) 
            return pthread_mutex_unlock(&c->m), SIM_STOP; 
    } 
    set_used(l); 
    set_used(r); 
    deq(&l->q); 
    deq(&r->q); 
    pthread_mutex_unlock(&c->m); 
    log_e(c, EVT_DONGLE); 
    log_e(c, EVT_DONGLE); 
    return SIM_RUN; 
}

void rel_d(t_coder *c) { 
    uint64_t now = ms(); 
    pthread_mutex_lock(&c->ld->m); 
    c->ld->used = 0; 
    c->ld->cool_end = now + *c->ld->cool; 
    pthread_mutex_unlock(&c->ld->m); 
    pthread_mutex_lock(&c->rd->m); 
    c->rd->used = 0; 
    c->rd->cool_end = now + *c->rd->cool; 
    pthread_mutex_unlock(&c->rd->m); 
    pthread_mutex_lock(&c->ln->m); 
    pthread_cond_broadcast(&c->ln->cv); 
    pthread_mutex_unlock(&c->ln->m); 
    pthread_mutex_lock(&c->rn->m); 
    pthread_cond_broadcast(&c->rn->cv); 
    pthread_mutex_unlock(&c->rn->m); 
}

void init_dl(t_coder *c, const t_config *cfg) { 
    if (DEADLINE_START == 0) 
        c->dl = UINT64_MAX; 
    else { 
        c->dl = cfg->sdm; 
        c->dts = cfg->sdts; 
    } 
}

void reset_dl(t_coder *c, uint64_t del) { 
    struct timeval now; 
    struct timespec *ts; 
    gettimeofday(&now, NULL); 
    pthread_mutex_lock(&c->m); 
    c->dl = now.tv_sec * 1000 + now.tv_usec / 1000 + del; 
    pthread_mutex_unlock(&c->m); 
    ts = &c->dts; 
    ts->tv_sec = now.tv_sec + del / 1000; 
    ts->tv_nsec = now.tv_usec * 1000 + (del % 1000 * 1000000); 
    if (ts->tv_nsec >= 1000000000) { 
        ts->tv_sec += ts->tv_nsec / 1000000000; 
        ts->tv_nsec %= 1000000000; 
    } 
}

bool alive(t_coder *c) { 
    bool a; 
    pthread_mutex_lock(&c->m); 
    a = ms() < c->dl; 
    pthread_mutex_unlock(&c->m); 
    return a; 
}

void upd_done(t_coder *c) { 
    pthread_mutex_lock(&c->m); 
    c->done++; 
    pthread_mutex_unlock(&c->m); 
}

enum e_sim exec_t(const t_coder *c, uint64_t dur) { 
    uint64_t end = ms() + dur; 
    while (ms() < end) { 
        if (!is_run() || ms() >= c->dl) 
            return SIM_STOP; 
        usleep(1000); 
    } 
    return SIM_RUN; 
}

void *coder_r(t_coder *c) { 
    const t_config *cfg = (const t_config *)(c->ld[0].cool - offsetof(t_config, dc) / sizeof(uint64_t)); 
    if (DEADLINE_START == 0) 
        reset_dl(c, cfg->ttb); 
    while (alive(c)) { 
        if (req_d(c) == SIM_STOP) 
            return NULL; 
        reset_dl(c, cfg->ttb); 
        log_e(c, EVT_COMPILE); 
        if (exec_t(c, cfg->ttc) == SIM_STOP) 
            return NULL; 
        rel_d(c); 
        upd_done(c); 
        log_e(c, EVT_DEBUG); 
        if (exec_t(c, cfg->ttd) == SIM_STOP) 
            return NULL; 
        log_e(c, EVT_REFACTOR); 
        if (exec_t(c, cfg->ttr) == SIM_STOP) 
            return NULL; 
    } 
    return NULL; 
}

void *mon_r(void *arg) { 
    t_data *d = (t_data *)arg; 
    const t_config *cfg = d->c; 
    uint32_t i, done; 
    while (1) { 
        i = 0; 
        done = 0; 
        while (i < cfg->nc) { 
            pthread_mutex_lock(&d->coders[i].m); 
            if (ms() >= d->coders[i].dl) { 
                pthread_mutex_unlock(&d->coders[i].m); 
                stop_sim(); 
                log_e(&d->coders[i], EVT_BURNOUT); 
                return NULL; 
            } 
            if (d->coders[i].done >= cfg->nc2) 
                done++; 
            pthread_mutex_unlock(&d->coders[i].m); 
            i++; 
        } 
        if (done == cfg->nc) 
            return stop_sim(), NULL; 
        usleep(1000); 
    } 
    return NULL; 
}

int init_d(t_data *d) { 
    uint32_t i = 0; 
    while (i < d->c->nc) { 
        if (pthread_mutex_init(&d->d[i].m, NULL) || qinit(&d->d[i].q)) 
            return -1; 
        d->d[i].cool = &d->c->dc; 
        d->d[i].cool_end = 0; 
        d->d[i].used = 0; 
        i++; 
    } 
    return 0; 
}

int init_c(t_data *d) { 
    static pthread_mutex_t lm = PTHREAD_MUTEX_INITIALIZER; 
    uint32_t i = 0; 
    while (i < d->c->nc) { 
        d->coders[i].id = i + 1; 
        if (pthread_mutex_init(&d->coders[i].m, NULL) || 
            pthread_cond_init(&d->coders[i].cv, NULL)) 
            return -1; 
        init_dl(&d->coders[i], d->c); 
        d->coders[i].done = 0; 
        d->coders[i].lm = &lm; 
        d->coders[i].ld = &d->d[i]; 
        d->coders[i].rd = &d->d[(i + 1) % d->c->nc]; 
        d->coders[i].ln = &d->coders[(i - 1 + d->c->nc) % d->c->nc]; 
        d->coders[i].rn = &d->coders[(i + 1) % d->c->nc]; 
        d->coders[i].enq = d->c->sch; 
        i++; 
    } 
    return 0; 
}

void set_ps(t_config *c) { 
    struct timeval now; 
    gettimeofday(&now, NULL); 
    c->psm = now.tv_sec * 1000 + now.tv_usec / 1000; 
    c->sdm = c->psm + c->ttb; 
    c->sdts.tv_sec = now.tv_sec + c->ttb / 1000; 
    c->sdts.tv_nsec = now.tv_usec * 1000 + (c->ttb % 1000 * 1000000); 
    if (c->sdts.tv_nsec >= 1000000000) { 
        c->sdts.tv_sec += c->sdts.tv_nsec / 1000000000; 
        c->sdts.tv_nsec %= 1000000000; 
    } 
}

int parse(int ac, char **av, t_config *c) { 
    if (ac != 9) 
        return -1; 
    c->nc = atoi(av[1]); 
    c->ttb = atoi(av[2]); 
    c->ttc = atoi(av[3]); 
    c->ttd = atoi(av[4]); 
    c->ttr = atoi(av[5]); 
    c->nc2 = atoi(av[6]); 
    c->dc = atoi(av[7]); 
    if (!strcmp(av[8], "fifo")) 
        c->sch = fifo_enq; 
    else if (!strcmp(av[8], "edf")) 
        c->sch = edf_enq; 
    else 
        return -1; 
    return 0; 
}

int start_c(t_data *d) { 
    uint32_t o = 0, i; 
    while (o < START_PATTERN && o < d->c->nc) { 
        i = o; 
        while (i < d->c->nc) { 
            if (pthread_create(&d->coders[i].th, NULL, 
                (void *(*)(void *))coder_r, &d->coders[i])) 
                return -1; 
            i += START_PATTERN; 
        } 
        usleep(1000); 
        o++; 
    } 
    return 0; 
}

void join_all(t_data *d) { 
    uint32_t i = 0; 
    while (i < d->c->nc) { 
        pthread_join(d->coders[i].th, NULL); 
        i++; 
    } 
    pthread_join(d->mt, NULL); 
}

void clean(t_data *d) { 
    uint32_t i; 
    if (!d->c) 
        return; 
    if (d->coders) { 
        i = 0; 
        while (i < d->c->nc) { 
            pthread_mutex_destroy(&d->coders[i].m); 
            pthread_cond_destroy(&d->coders[i].cv); 
            qdest(&d->d[i].q); 
            pthread_mutex_destroy(&d->d[i].m); 
            i++; 
        } 
        free(d->coders); 
        free(d->d); 
    } 
    free(d->c); 
}

int main(int ac, char **av) { 
    t_data d; 
    if (ac == 2 && !strcmp(av[1], "--help")) 
        return printf("Usage\n"), 0; 
    d.c = malloc(sizeof(t_config)); 
    if (!d.c || parse(ac, av, d.c)) 
        return free(d.c), 1; 
    if (!d.c->nc || d.c->nc == 1 || !d.c->nc2) { 
        if (d.c->nc == 1) { 
            usleep(d.c->ttb * 1000); 
            printf("%lu 1 burned out\n", d.c->ttb); 
        } 
        return free(d.c), 0; 
    } 
    d.coders = malloc(sizeof(t_coder) * d.c->nc); 
    d.d = malloc(sizeof(t_dongle) * d.c->nc); 
    if (!d.coders || !d.d) 
        return clean(&d), 1; 
    set_ps(d.c); 
    if (init_d(&d) || init_c(&d)) 
        return clean(&d), 1; 
    if (pthread_create(&d.mt, NULL, mon_r, &d)) 
        return clean(&d), 1; 
    if (start_c(&d)) 
        return clean(&d), 1; 
    join_all(&d); 
    clean(&d); 
    return 0; 
}
