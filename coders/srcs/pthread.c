/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthread.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 23:42:04 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/07 00:31:30 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void* myThreadFunc(void* arg)
{
	pthread_t	thisThread = pthread_self();
	int 		v = *(int*)arg;

	printf("Thread create: %d, ID: %lu\n", v, (unsigned long)thisThread);
	return NULL;
}

void	create_threads(const char *argv[])
{
	pthread_t	threads[atoi(argv[1])];
	int			num_thread;
	int			i;
	int			j;

	num_thread = atoi(argv[1]);

	i = 0;
	while(i < num_thread)
	{
		pthread_create(&threads[i], NULL, myThreadFunc, &i);
		i++;
	}

	j = 0;
	while (j < num_thread)
	{
		pthread_join(threads[j], NULL);
		j++;
	}
}
