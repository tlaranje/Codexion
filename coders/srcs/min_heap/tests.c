/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 10:49:15 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/23 11:10:28 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "min_heap.h"
#include "utils.h"

/* int	main(int argc, char **argv)
{
	t_heap	heap;
	t_coder	coders[100];
	int		i;

	if (argc < 2)
		return (1);
	heap.size = 0;
	i = 1;
	while (i < argc)
	{
		coders[i - 1].deadline = (unsigned long)atoi(argv[i]);
		heap.data[heap.size++] = &coders[i - 1];
		heapify_up_edf(&heap, heap.size - 1);
		i++;
	}
	while (heap.size > 0)
		printf("%lu\n", heap_pop(&heap, "edf")->deadline);
	return (0);
} */

/* int	main(int argc, char **argv)
{
	t_heap			heap;
	t_coder			coders[100];
	t_coder			*c;
	uint32_t		order;
	int				i;

	if (argc < 2)
		return (1);
	heap.size = 0;
	order = 0;
	i = 1;
	while (i < argc)
	{
		coders[i - 1].id = atoi(argv[i]);
		coders[i - 1].arrival_order = order++;
		heap.data[heap.size++] = &coders[i - 1];
		heapify_up_fifo(&heap, heap.size - 1);
		i++;
	}
	while (heap.size > 0)
	{
		c = heap_pop(&heap, "fifo");
		printf("id=%d order=%u\n", c->id, c->arrival_order);
	}
	return (0);
} */
