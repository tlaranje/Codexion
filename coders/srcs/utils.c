/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:19:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/11 16:52:21 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	malloc_structs(t_data *data, int argc, const char *argv[])
{
	data->config = malloc(sizeof(t_config));
	if (init_config(data->config, argc, argv) != 0)
		return (-1);
	data->monitor = malloc(sizeof(t_monitor));
	data->coders = malloc(sizeof(t_coder) * data->config->num_coders);
	data->dongles = malloc(sizeof(t_dongle) * data->config->num_coders);
	data->args = malloc(sizeof(t_thread_args) * data->config->num_coders);
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
