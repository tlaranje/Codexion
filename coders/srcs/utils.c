/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 10:19:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/10 17:24:17 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_thread_args(uint32_t i, t_data *d)
{
	d->args[i].config = d->config;
	d->args[i].monitor = d->monitor;
	d->args[i].coder = &d->coders[i];
	d->args[i].dongle = &d->dongles[i];
	return (0);
}

int	init_config(t_config *config, int argc, const char *argv[])
{
	if (check_args(argc, argv) != 0)
		return (-1);
	config->num_coders = atoi(argv[1]);
	config->time_to_burnout = atoi(argv[2]);
	config->time_to_compile = atoi(argv[3]);
	config->time_to_debug = atoi(argv[4]);
	config->time_to_refactor = atoi(argv[5]);
	config->num_compiles = atoi(argv[6]);
	config->dongle_cooldown = atoi(argv[7]);
	config->scheduler = (char *)argv[8];
	return (0);
}

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

int	init_coder_dongle(t_data *d)
{
	uint32_t	i;

	i = -1;
	while (++i < d->config->num_coders)
	{
		d->coders[i].id = i + 1;
		d->dongles[i].id = i + 1;
		d->dongles[i].cooldown = d->config->dongle_cooldown;
		d->dongles[i].is_in_use = false;
		d->coders[i].left_dongle = &d->dongles[i];
		d->coders[i].right_dongle = &d->dongles[(i + 1) % d->config->num_coders];
		d->coders[i].last_compile_start = d->monitor->start_time;
	}
	return (0);
}

