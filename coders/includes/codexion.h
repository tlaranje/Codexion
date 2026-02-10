/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:49:00 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/10 17:41:03 by tlaranje         ###   ########.fr       */
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

// Structs
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

typedef struct s_dongle
{
	uint32_t		id;
	uint32_t		cooldown;
	bool			is_in_use;
	pthread_mutex_t	dongle_mutex;
	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_coder
{
	uint32_t	id;
	pthread_t	thread;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;

	uint64_t	last_compile_start;
	uint32_t	compile_count;
}	t_coder;


typedef struct s_monitor
{
	pthread_mutex_t	log_mutex;
	pthread_t		thread;
	uint64_t		start_time;
	t_config		*config;
	t_coder			*coders;
	bool 			stop;
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

// check_args.c
int			check_args(int argc, const char *argv[]);

// time_utils.c
uint64_t	get_time_ms(void);

// coder.c
void		start_threads(t_data *d);
void		join_threads(t_data *d);

// coder_utils.c
int			take_dongle(t_dongle *d, t_monitor *m, t_coder *c);
int			free_dongle(t_dongle *dongle);
void		compiling(t_thread_args *ta);
void		debugging(t_thread_args *ta);
void		refactoring(t_thread_args *ta);

// Utils.c
int			init_config(t_config *config, int argc, const char *argv[]);
int			malloc_structs(t_data *data, int argc, const char *argv[]);
int			init_coder_dongle(t_data *d);
int			init_thread_args(uint32_t i, t_data *d);

#endif
