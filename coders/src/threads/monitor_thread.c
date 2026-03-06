/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:03:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/03/06 10:38:44 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"
#include "codexion.h"
#include "utils.h"

static bool	check_or_set_stop(t_monitor *monitor, bool set)
{
	bool	stop;

	pthread_mutex_lock(&monitor->monitor_mutex);
	if (set)
		monitor->stop = true;
	stop = monitor->stop;
	pthread_mutex_unlock(&monitor->monitor_mutex);
	return (stop);
}

static void	handle_dead(t_data *d, int dead, uint64_t now)
{
	uint32_t	i;

	pthread_mutex_lock(&d->monitor->log_mutex);
	printf("%lu %d burned out\n", now, d->coders[dead].id);
	pthread_mutex_unlock(&d->monitor->log_mutex);
	check_or_set_stop(d->monitor, true);
	i = -1;
	while (++i < d->config->num_coders)
	{
		pthread_mutex_lock(&d->dongles[i].dongle_mutex);
		pthread_cond_broadcast(&d->dongles[i].dongle_cond);
		pthread_mutex_unlock(&d->dongles[i].dongle_mutex);
	}
}

static int	find_next_burnout(t_data *d, uint64_t now)
{
	int			i;
	uint64_t	diff;

	i = 0;
	while (i < (int)d->config->num_coders)
	{
		pthread_mutex_lock(&d->coders[i].coder_mutex);
		if (!d->coders[i].compile_count)
			return (pthread_mutex_unlock(&d->coders[i].coder_mutex), i++, -1);
		diff = now - d->coders[i].last_compile_start;
		pthread_mutex_unlock(&d->coders[i].coder_mutex);
		if (diff >= d->config->time_to_burnout)
			return (i);
		i++;
	}
	return (-1);
}

static bool	all_coders_finished(t_data *d)
{
	uint32_t	i;
	uint32_t	finished;

	finished = 0;
	i = 0;
	while (i < d->config->num_coders)
	{
		pthread_mutex_lock(&d->coders[i].coder_mutex);
		if (d->coders[i].compile_count >= d->config->num_compiles)
			finished++;
		pthread_mutex_unlock(&d->coders[i].coder_mutex);
		i++;
	}
	return (finished == d->config->num_coders);
}

void	*monitor_routine(void *arg)
{
	t_data		*d;
	int			dead;
	uint64_t	now;

	d = (t_data *)arg;
	while (1)
	{
		if (check_or_set_stop(d->monitor, false))
			return (NULL);
		if (all_coders_finished(d))
			return (check_or_set_stop(d->monitor, true), NULL);
		now = get_time_ms() - d->monitor->start_time;
		dead = find_next_burnout(d, now);
		if (dead >= 0)
			return (handle_dead(d, dead, now), NULL);
		usleep(1000);
	}
}
