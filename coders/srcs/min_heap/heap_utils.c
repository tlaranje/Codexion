/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:03:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 14:34:04 by tlaranje         ###   ########.fr       */
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

int	heap_push(t_heap *h, t_coder *c, const char *mode)
{
	if (h->size >= MAX_CODERS)
		return (-1);
	h->data[h->size] = c;
	if (strcmp(mode, "fifo") == 0)
		heapify_up_fifo(h, h->size);
	else if (strcmp(mode, "edf") == 0)
		heapify_up_edf(h, h->size);
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
	if (strcmp(mode, "fifo") == 0)
		heapify_down_fifo(h, 0);
	else if (strcmp(mode, "edf") == 0)
		heapify_down_edf(h, 0);
	return (top);
}

t_coder	*heap_peek(t_heap *h)
{
	if (h->size == 0)
		return (NULL);
	return (h->data[0]);
}
