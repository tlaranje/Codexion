/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:03:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/23 17:09:20 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"
#include "codexion.h"
#include "utils.h"
#include <stdio.h>

void	add_to_wait_queue(t_thread_args *ta)
{
	t_coder	*coder;

	coder = ta->coder;
	pthread_mutex_lock(&ta->coder->coder_mutex);
	coder->deadline = coder->last_compile_start + ta->config->time_to_burnout;
	coder->arrival_order = ta->coder->id;
	pthread_mutex_lock(&ta->monitor->monitor_mutex);
	heap_push(ta, ta->config->scheduler);
	pthread_mutex_unlock(&ta->monitor->monitor_mutex);
	pthread_mutex_unlock(&ta->coder->coder_mutex);
}

static void	do_action(t_thread_args *ta, const char *action, uint64_t duration)
{
	uint64_t	now;

	now = get_time_ms() - ta->monitor->start_time;
	if (strcmp(action, "compiling") == 0)
	{
		pthread_mutex_lock(&ta->coder->coder_mutex);
		ta->coder->last_compile_start = get_time_ms() - ta->monitor->start_time;
		pthread_mutex_unlock(&ta->coder->coder_mutex);
		ta->coder->compile_count++;
	}
	pthread_mutex_lock(&ta->monitor->monitor_mutex);
	pthread_mutex_lock(&ta->monitor->log_mutex);
	if (!ta->monitor->stop)
		printf("%lu %d is %s\n", now, ta->coder->id, action);
	pthread_mutex_unlock(&ta->monitor->log_mutex);
	pthread_mutex_unlock(&ta->monitor->monitor_mutex);
	usleep(duration * 1000);
}

static void	do_coder_actions(t_thread_args *ta)
{
	add_to_wait_queue(ta);
	take_two_dongles(ta, ta->coder);
	do_action(ta, "compiling", ta->config->time_to_compile);
	free_two_dongles(ta, ta->coder);
	do_action(ta, "debugging", ta->config->time_to_debug);
	do_action(ta, "refactoring", ta->config->time_to_refactor);
}

void	*coder_routine(void *arg)
{
	t_thread_args	*ta;
	bool			stop;

	ta = (t_thread_args *)arg;
	while (1)
	{
		pthread_mutex_lock(&ta->monitor->monitor_mutex);
		stop = ta->monitor->stop;
		pthread_mutex_unlock(&ta->monitor->monitor_mutex);
		if (stop)
			break ;
		do_coder_actions(ta);
		pthread_mutex_lock(&ta->monitor->monitor_mutex);
		if (ta->coder->compile_count == ta->config->num_compiles)
			ta->monitor->stop = true;
		pthread_mutex_unlock(&ta->monitor->monitor_mutex);
	}
	return (NULL);
}
