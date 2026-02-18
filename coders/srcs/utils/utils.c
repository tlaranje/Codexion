/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:19:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 16:44:33 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils.h"
#include "min_heap.h"

int	malloc_structs(t_data *d, int argc, const char *argv[])
{
	uint32_t	num_coders;

	d->config = malloc(sizeof(t_config));
	if (!d->config)
		return (-1);
	if (init_config(d->config, argc, argv) != 0)
		return (-1);
	num_coders = d->config->num_coders;
	d->heap = malloc(sizeof(t_heap));
	if (!d->heap)
		return (-1);
	d->monitor = malloc(sizeof(t_monitor));
	if (!d->monitor)
		return (-1);
	d->coders = malloc(sizeof(t_coder) * num_coders);
	if (!d->coders)
		return (-1);
	d->dongles = malloc(sizeof(t_dongle) * num_coders);
	if (!d->dongles)
		return (-1);
	d->args = malloc(sizeof(t_thread_args) * num_coders);
	if (!d->args)
		return (-1);
	return (0);
}

int	free_all(t_data *d)
{
	pthread_mutex_destroy(&d->monitor->monitor_mutex);
	pthread_mutex_destroy(&d->monitor->log_mutex);
	pthread_mutex_destroy(&d->dongles->dongle_mutex);
	pthread_cond_destroy(&d->dongles->dongle_cond);
	free(d->coders);
	free(d->dongles);
	free(d->args);
	free(d->monitor);
	free(d->config);
	return (0);
}

uint64_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
