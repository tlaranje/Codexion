/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 10:49:15 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/25 15:33:05 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "min_heap.h"
#include "utils.h"

#define CODERS 3

void	fill_heap(t_heap *h, t_coder coders[CODERS])
{
	int	i;

	h->size = 0;
	i = 0;
	while (i < CODERS)
	{
		coders[i].id = i;
		coders[i].deadline = rand() % 10;
		coders[i].arrival_order = i;
		h->data[h->size++] = &coders[i];
		i++;
	}
	coders[2].deadline = coders[0].deadline;
}

void	print_heap(t_heap *h)
{
	int	i;

	printf("Heap current state:\n");
	i = 0;
	while (i < h->size)
	{
		printf("Coder[%d] - Deadline: %lu - Arrival: %d\n",
			h->data[i]->id, h->data[i]->deadline,
			h->data[i]->arrival_order);
		i++;
	}
}

void	test_edf(t_heap *h)
{
	t_heap	h_copy;
	t_coder	*top;
	int		i;

	h_copy = *h;
	i = 0;
	while (i < h_copy.size)
	{
		heapify_up_edf(&h_copy, i);
		i++;
	}
	while (h_copy.size > 0)
	{
		top = h_copy.data[0];
		h_copy.data[0] = h_copy.data[--h_copy.size];
		heapify_down_edf(&h_copy, 0);
		printf("EDF - Coder[%d] - Deadline: %lu\n",
			top->id, top->deadline);
	}
}

void	test_fifo(t_heap *h)
{
	t_heap	h_copy;
	t_coder	*top;
	int		i;

	h_copy = *h;
	i = 0;
	while (i < h_copy.size)
	{
		heapify_up_fifo(&h_copy, i);
		i++;
	}
	while (h_copy.size > 0)
	{
		top = h_copy.data[0];
		h_copy.data[0] = h_copy.data[--h_copy.size];
		heapify_down_fifo(&h_copy, 0);
		printf("FIFO - Coder[%d] - Arrival: %d\n",
			top->id, top->arrival_order);
	}
}

/* int	main(void)
{
	t_heap	h;
	t_coder	coders[CODERS];

	pthread_mutex_init(&h.heap_mutex, NULL);
	srand(time(NULL));
	fill_heap(&h, coders);
	print_heap(&h);
	printf("\nHeap after EDF:\n");
	test_edf(&h);
	printf("\nHeap after FIFO:\n");
	test_fifo(&h);
	pthread_mutex_destroy(&h.heap_mutex);
	return (0);
} */
