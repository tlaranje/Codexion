/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:19:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/16 14:10:12 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	malloc_structs(t_data *d, int argc, const char *argv[])
{
	uint32_t	num_coders;

	d->config = malloc(sizeof(t_config));
	if (init_config(d->config, argc, argv) != 0)
		return (-1);
	num_coders = d->config->num_coders;
	d->monitor = malloc(sizeof(t_monitor));
	d->coders = malloc(sizeof(t_coder) * num_coders);
	d->dongles = malloc(sizeof(t_dongle) * num_coders);
	d->args = malloc(sizeof(t_thread_args) * num_coders);
	return (0);
}

int	free_all(t_data *d)
{
	pthread_mutex_destroy(&d->monitor->mutex);
	pthread_mutex_destroy(&d->monitor->log_mutex);
	pthread_mutex_destroy(&d->monitor->dongle_mutex);
	pthread_cond_destroy(&d->monitor->dongle_cond);
	free(d->coders);
	free(d->dongles);
	free(d->args);
	free(d->monitor);
	free(d->config);
	return (0);
}
