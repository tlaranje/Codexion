/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_edf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 10:58:07 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 14:05:18 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"

void	heapify_up_edf(t_heap *h, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (h->data[i]->deadline
			>= h->data[parent]->deadline)
			break ;
		swap(&h->data[i], &h->data[parent]);
		i = parent;
	}
}

void	heapify_down_edf(t_heap *h, int i)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < h->size
			&& h->data[left]->deadline
			< h->data[smallest]->deadline)
			smallest = left;
		if (right < h->size
			&& h->data[right]->deadline
			< h->data[smallest]->deadline)
			smallest = right;
		if (smallest == i)
			break ;
		swap(&h->data[i], &h->data[smallest]);
		i = smallest;
	}
}
