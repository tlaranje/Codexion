/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:49:30 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/23 16:42:20 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "codexion.h"
#include "check_args.h"
#include "utils.h"

int	init_coder_dongle(t_data *d)
{
	uint32_t	i;
	uint32_t	next;

	i = 0;
	while (i < d->config->num_coders)
	{
		d->coders[i].id = i + 1;
		d->dongles[i].id = i + 1;
		d->dongles[i].cooldown = d->config->dongle_cooldown;
		d->dongles[i].in_use = false;
		pthread_mutex_init(&d->coders[i].coder_mutex, NULL);
		pthread_mutex_init(&d->dongles[i].dongle_mutex, NULL);
		pthread_cond_init(&d->dongles[i].dongle_cond, NULL);
		d->args[i].heap = d->heap;
		d->coders[i].left_dongle = &d->dongles[i];
		next = (i + 1) % d->config->num_coders;
		d->coders[i].right_dongle = &d->dongles[next];
		d->coders[i].last_compile_start = 0;
		d->coders[i].compile_count = 0;
		d->dongles[i].available_at = 0;
		i++;
	}
	return (0);
}

int	init_mutexes(t_data *d)
{
	pthread_mutex_init(&d->heap->heap_mutex, NULL);
	pthread_mutex_init(&d->monitor->monitor_mutex, NULL);
	pthread_mutex_init(&d->monitor->log_mutex, NULL);
	d->heap->size = 0;
	d->monitor->stop = false;
	d->monitor->start_time = get_time_ms();
	return (0);
}

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
