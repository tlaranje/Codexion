/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_fifo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 10:58:11 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/25 11:52:58 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"

void	heapify_up_fifo(t_heap *h, int i)
{
	int	parent;

	pthread_mutex_lock(&h->heap_mutex);
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (h->data[i]->arrival_order
			>= h->data[parent]->arrival_order)
			break ;
		swap(&h->data[i], &h->data[parent]);
		i = parent;
	}
	pthread_mutex_unlock(&h->heap_mutex);
}

void	heapify_down_fifo(t_heap *h, int i)
{
	int	left;
	int	right;
	int	smallest;

	pthread_mutex_lock(&h->heap_mutex);
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < h->size
			&& h->data[left]->arrival_order
			< h->data[smallest]->arrival_order)
			smallest = left;
		if (right < h->size
			&& h->data[right]->arrival_order
			< h->data[smallest]->arrival_order)
			smallest = right;
		if (smallest == i)
			break ;
		swap(&h->data[i], &h->data[smallest]);
		i = smallest;
	}
	pthread_mutex_unlock(&h->heap_mutex);
}
