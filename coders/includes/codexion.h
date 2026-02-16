/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:49:00 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/16 16:48:26 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdint.h>
# include <stdbool.h>

// Macros
#define MAX_CODERS 200

// Structs
typedef struct s_dongle
{
	uint32_t		id;
	uint32_t		cooldown;
	bool			in_use;
}	t_dongle;

typedef struct s_coder
{
	uint32_t		id;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	uint64_t		last_compile_start;
	uint32_t		compile_count;
	uint64_t		deadline;
	uint32_t		arrival_order;
}	t_coder;

typedef struct s_heap
{
	t_coder *data[MAX_CODERS];
	int size;
}	t_heap;

typedef struct s_config
{
	uint32_t	num_coders;
	uint64_t	time_to_burnout;
	uint64_t	time_to_compile;
	uint64_t	time_to_debug;
	uint64_t	time_to_refactor;
	uint64_t	num_compiles;
	uint32_t	dongle_cooldown;
	char		*scheduler;
}	t_config;

typedef struct s_monitor
{
	t_heap			wait_heap;
	pthread_mutex_t	dongle_mutex;
	pthread_mutex_t	heap_mutex;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	mutex;
	pthread_cond_t	dongle_cond;
	pthread_t		thread;
	uint64_t		start_time;
	t_config		*config;
	t_coder			*coders;
	bool			stop;
}	t_monitor;

typedef struct s_thread_args
{
	t_config	*config;
	t_monitor	*monitor;
	t_coder		*coder;
	t_dongle	*dongle;
}	t_thread_args;

typedef struct s_data
{
	t_config		*config;
	t_monitor		*monitor;
	t_coder			*coders;
	t_dongle		*dongles;
	t_thread_args	*args;
}	t_data;

// main.c
uint64_t	get_time_ms(void);

// monitor_routime.c
void		*monitor_routime(void *arg);

// check_args.c
int			check_args(int argc, const char *argv[]);

// coder.c
void		start_threads(t_data *d);
void		join_threads(t_data *d);

// coder_utils.c
int			take_two_dongles(t_coder *c, t_monitor *m);
int			free_two_dongles(t_coder *c, t_monitor *m);
void		do_action(t_thread_args *ta, const char *action, uint64_t duration);

// init_utils.c
int			init_config(t_config *config, int argc, const char *argv[]);
int			init_coder_dongle(t_data *d);
int			init_thread_args(uint32_t i, t_data *d);
int			init_monitor_mutexes(t_monitor *m);

// utils.c
int			free_all(t_data *d);
int			malloc_structs(t_data *data, int argc, const char *argv[]);

// dongle_scheduler.c
void		add_to_wait_queue(t_thread_args *ta);

// heap.c
int			heap_push(t_heap *h, t_coder *c, const char *mode);
t_coder		*heap_pop(t_heap *h, const char *mode);
t_coder		*heap_peek(t_heap *h);

#endif
