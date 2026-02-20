/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:49:00 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/20 16:11:44 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIN_HEAP_H
# define MIN_HEAP_H

// Includes
# include <pthread.h>
#include "threads.h"

// Forward declarations
typedef struct s_heap t_heap;
typedef struct s_coder t_coder;

// Macros
# define MAX_CODERS 200

// Structs
struct s_heap
{
	pthread_mutex_t	heap_mutex;
	t_coder	*data[MAX_CODERS];
	int		size;
};

// heap_edf.c
void	heapify_up_edf(t_heap *h, int i);
void	heapify_down_edf(t_heap *h, int i);

// heap_fifo.c
void	heapify_up_fifo(t_heap *h, int i);
void	heapify_down_fifo(t_heap *h, int i);

// heap_utlis.c
void	swap(t_coder **a, t_coder **b);
int		heap_push(t_thread_args *ta, const char *mode);
t_coder	*heap_pop(t_thread_args *ta, const char *mode);
t_coder	*heap_peek(t_thread_args *ta);

#endif