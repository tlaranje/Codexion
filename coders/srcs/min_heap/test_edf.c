/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_edf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 15:08:07 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/25 15:32:40 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"

static int	edf_cmp(t_coder *a, t_coder *b, int order)
{
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	if (order == 0)
		return (a->id < b->id);
	return (a->id > b->id);
}

void	t_heapify_up_edf(t_heap *h, int i, int order)
{
	int	parent;

	pthread_mutex_lock(&h->heap_mutex);
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!edf_cmp(h->data[i], h->data[parent], order))
			break ;
		swap(&h->data[i], &h->data[parent]);
		i = parent;
	}
	pthread_mutex_unlock(&h->heap_mutex);
}

void	t_heapify_down_edf(t_heap *h, int i, int order)
{
	t_coder	**data;
	int		left;
	int		right;
	int		small;

	pthread_mutex_lock(&h->heap_mutex);
	data = h->data;
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		small = i;
		if (left < h->size && edf_cmp(h->data[left], h->data[small], order))
			small = left;
		if (right < h->size && edf_cmp(h->data[right], h->data[small], order))
			small = right;
		if (small == i)
			break ;
		swap(&data[i], &data[small]);
		i = small;
	}
	pthread_mutex_unlock(&h->heap_mutex);
}

/*
Add this to heapify_up_edf and  heapify_down_edf so that when the deadlines
are the same for prioritize the coder with the highest ID
---
static int	edf_cmp(t_coder *a, t_coder *b)
{
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	return (a->id > b->id);
}
---
Chance the if of heapify_up_edf to
if (!edf_cmp(h->data[i], h->data[parent]))
	break;

Chance the ifs of heapify_down_edf to
if (left < h->size && edf_cmp(h->data[left], h->data[smallest]))
	smallest = left;

if (right < h->size && edf_cmp(h->data[right], h->data[smallest]))
	smallest = right;
*/