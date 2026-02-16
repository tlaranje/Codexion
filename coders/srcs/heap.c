/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:03:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/16 17:00:45 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap(t_coder **a, t_coder **b)
{
	t_coder	*tmp;
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	heapify_up(t_heap *h, int i, const char *mode)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if ((strcmp(mode, "fifo") == 0 && h->data[i]->arrival_order >= \
				h->data[parent]->arrival_order) ||
			(strcmp(mode, "edf") == 0 && h->data[i]->deadline >= \
				h->data[parent]->deadline))
			break ;
		swap(&h->data[i], &h->data[parent]);
		i = parent;
	}
}

static void	heapify_down(t_heap *h, int i, const char *mode)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < h->size && ((strcmp(mode, "fifo") == 0 && \
				h->data[left]->arrival_order < h->data[smallest]->arrival_order) || \
			(strcmp(mode, "edf") == 0 && h->data[left]->deadline < \
				h->data[smallest]->deadline)))
			smallest = left;
		if (right < h->size && ((strcmp(mode, "fifo") == 0 && \
				h->data[right]->arrival_order < h->data[smallest]->arrival_order) || \
			(strcmp(mode, "edf") == 0 && h->data[right]->deadline < \
				h->data[smallest]->deadline)))
			smallest = right;
		if (smallest == i)
			break ;
		swap(&h->data[i], &h->data[smallest]);
		i = smallest;
	}
}

int	heap_push(t_heap *h, t_coder *c, const char *mode)
{
	if (h->size >= MAX_CODERS)
		return (-1);
	h->data[h->size] = c;
	heapify_up(h, h->size, mode);
	h->size++;
	return (0);
}

t_coder	*heap_pop(t_heap *h, const char *mode)
{
	t_coder	*top;

	if (h->size == 0)
		return (NULL);
	top = h->data[0];
	h->data[0] = h->data[h->size - 1];
	h->size--;
	heapify_down(h, 0, mode);
	return (top);
}

t_coder	*heap_peek(t_heap *h)
{
	if (h->size == 0)
		return (NULL);
	return (h->data[0]);
}
