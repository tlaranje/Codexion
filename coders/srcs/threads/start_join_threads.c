/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_join_threads.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:03:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 16:13:59 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"
#include "codexion.h"
#include "utils.h"

void	start_threads(t_data *d)
{
	uint32_t	i;

	i = -1;
	while (++i < d->config->num_coders)
	{
		init_thread_args(i, d);
		pthread_create(&d->coders[i].thread, NULL, coder_routime, &d->args[i]);
	}
	pthread_create(&d->monitor->thread, NULL, monitor_routime, d);
	usleep(1000);
}

void	join_threads(t_data *d)
{
	uint32_t	i;

	i = -1;
	while (++i < d->config->num_coders)
		pthread_join(d->coders[i].thread, NULL);
	pthread_join(d->monitor->thread, NULL);
}