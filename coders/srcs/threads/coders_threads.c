/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:03:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 17:37:58 by tlaranje         ###   ########.fr       */
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
	pthread_mutex_lock(&ta->heap->heap_mutex);
	coder->deadline = coder->last_compile_start + ta->config->time_to_burnout;
	coder->arrival_order = ta->coder->id;
	heap_push(ta->monitor->wait_heap, coder, ta->config->scheduler);
	pthread_mutex_unlock(&ta->heap->heap_mutex);
}

void	*coder_routime(void *arg)
{
	t_thread_args	*ta;

	ta = (t_thread_args *)arg;
	while (!ta->monitor->stop)
	{
		pthread_mutex_lock(&ta->monitor->monitor_mutex);
		ta->coder->last_compile_start = get_time_ms() - ta->monitor->start_time;
		pthread_mutex_unlock(&ta->monitor->monitor_mutex);
		add_to_wait_queue(ta);
		take_two_dongles(ta, ta->coder);
		do_action(ta, "compiling", ta->config->time_to_compile);
		free_two_dongles(ta, ta->coder);
		do_action(ta, "debugging", ta->config->time_to_debug);
		do_action(ta, "refactoring", ta->config->time_to_refactor);
		pthread_mutex_lock(&ta->coder->coder_mutex);
		pthread_mutex_lock(&ta->monitor->monitor_mutex);
		if (ta->coder->compile_count == ta->config->num_compiles)
			ta->monitor->stop = true;
		pthread_mutex_unlock(&ta->monitor->monitor_mutex);
		pthread_mutex_unlock(&ta->coder->coder_mutex);
	}
	return (NULL);
}
