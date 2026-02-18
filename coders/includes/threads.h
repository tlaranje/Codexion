/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:49:00 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 17:05:39 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_H
# define THREADS_H

// Includes
# include <stdint.h>
# include <stdbool.h>
# include <pthread.h>

// Forward declarations
typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;
typedef struct s_monitor t_monitor;
typedef struct s_thread_args t_thread_args;
typedef struct s_heap t_heap;
typedef struct s_config t_config;
typedef struct s_data t_data;

// Structs
struct s_dongle
{
	uint32_t		id;
	uint32_t		cooldown;
	bool			in_use;
	pthread_mutex_t	dongle_mutex;
	pthread_cond_t	dongle_cond;
};

struct s_coder
{
	pthread_mutex_t	coder_mutex;
	uint32_t	id;
	pthread_t	thread;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	uint64_t	last_compile_start;
	uint32_t	compile_count;
	uint64_t	deadline;
	uint32_t	arrival_order;
};

struct s_monitor
{
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	monitor_mutex;
	t_heap			*wait_heap;
	pthread_t		thread;
	uint64_t		start_time;
	t_config		*config;
	t_coder			*coders;
	bool			stop;
};

struct s_thread_args
{
	t_config	*config;
	t_monitor	*monitor;
	t_coder		*coder;
	t_dongle	*dongle;
	t_heap		*heap;
};

// coders_threads.c
void	*coder_routime(void *arg);
void	add_to_wait_queue(t_thread_args *ta);

// monitor_thread.c
void	*monitor_routime(void *arg);

// start_join_threads.c
void	start_threads(t_data *d);
void	join_threads(t_data *d);

// coders_utils.c
int		take_two_dongles(t_thread_args *ta, t_coder *c);
int		free_two_dongles(t_thread_args *ta, t_coder *c);
void	do_action(t_thread_args *ta, const char *action, uint64_t duration);

#endif