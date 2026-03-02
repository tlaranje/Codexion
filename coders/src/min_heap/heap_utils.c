/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:03:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/23 16:41:27 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"
#include "utils.h"

void	swap(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	heap_push(t_thread_args *ta, const char *mode)
{
	t_heap	*heap;

	heap = ta->monitor->wait_heap;
	if (heap->size < MAX_CODERS)
	{
		heap->data[heap->size] = ta->coder;
		if (strcmp(mode, "fifo") == 0)
			heapify_up_fifo(heap, heap->size);
		else if (strcmp(mode, "edf") == 0)
			heapify_up_edf(heap, heap->size);
		heap->size++;
		return (0);
	}
	return (-1);
}

t_coder	*heap_pop(t_thread_args *ta, const char *mode)
{
	t_heap	*heap;
	t_coder	*top;

	heap = ta->monitor->wait_heap;
	top = NULL;
	if (heap->size > 0)
	{
		top = heap->data[0];
		heap->data[0] = heap->data[heap->size - 1];
		heap->size--;
		if (strcmp(mode, "fifo") == 0)
			heapify_down_fifo(heap, 0);
		else if (strcmp(mode, "edf") == 0)
			heapify_down_edf(heap, 0);
	}
	return (top);
}

t_coder	*heap_peek(t_thread_args *ta)
{
	t_heap	*heap;
	t_coder	*top;

	heap = ta->monitor->wait_heap;
	top = NULL;
	if (heap->size > 0)
		top = heap->data[0];
	return (top);
}
