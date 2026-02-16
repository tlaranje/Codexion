/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_scheduler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 12:01:54 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/16 16:51:17 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void print_wait_queue(t_monitor *m)
{
	int i;

	pthread_mutex_lock(&m->heap_mutex);
	printf("WAIT_QUEUE ORDER: ");
	for (i = 0; i < m->wait_heap.size; i++)
	{
		t_coder *c = m->wait_heap.data[i];
		printf("coder[%d](deadline=%lu)", c->id, c->deadline);
		if (i != m->wait_heap.size - 1)
			printf(" -> ");
	}
	printf("\n");
	pthread_mutex_unlock(&m->heap_mutex);
}

void add_to_wait_queue(t_thread_args *ta)
{
	t_coder *coder;

	coder = ta->coder;
	pthread_mutex_lock(&ta->monitor->heap_mutex);
	coder->deadline = coder->last_compile_start + ta->config->time_to_burnout;
	coder->arrival_order = ta->coder->id;
	heap_push(&ta->monitor->wait_heap, coder, ta->config->scheduler);
	pthread_mutex_unlock(&ta->monitor->heap_mutex);
}
