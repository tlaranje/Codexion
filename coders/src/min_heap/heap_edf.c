/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_edf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 10:58:07 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/25 15:09:00 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"

void	heapify_up_edf(t_heap *h, int i)
{
	int	parent;

	pthread_mutex_lock(&h->heap_mutex);
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (h->data[i]->deadline >= h->data[parent]->deadline)
			break ;
		swap(&h->data[i], &h->data[parent]);
		i = parent;
	}
	pthread_mutex_unlock(&h->heap_mutex);
}

void	heapify_down_edf(t_heap *h, int i)
{
	t_coder	**data;
	int		left;
	int		right;
	int		smallest;

	pthread_mutex_lock(&h->heap_mutex);
	data = h->data;
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < h->size && data[left]->deadline < data[smallest]->deadline)
			smallest = left;
		if (right < h->size && data[right]->deadline < data[smallest]->deadline)
			smallest = right;
		if (smallest == i)
			break ;
		swap(&data[i], &data[smallest]);
		i = smallest;
	}
	pthread_mutex_unlock(&h->heap_mutex);
}
