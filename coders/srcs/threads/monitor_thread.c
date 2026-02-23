/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:03:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/23 17:39:04 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"
#include "codexion.h"
#include "utils.h"

static void	wake_all_dongles(t_data *d)
{
	uint32_t	i;

	i = -1;
	while (++i < d->config->num_coders)
	{
		pthread_mutex_lock(&d->dongles[i].dongle_mutex);
		pthread_cond_broadcast(&d->dongles[i].dongle_cond);
		pthread_mutex_unlock(&d->dongles[i].dongle_mutex);
	}
}

static void	handle_dead(t_data *d, int dead, uint64_t now)
{
	pthread_mutex_lock(&d->monitor->log_mutex);
	printf("%lu %d burned out\n", now, d->coders[dead].id);
	pthread_mutex_unlock(&d->monitor->log_mutex);
	pthread_mutex_lock(&d->monitor->monitor_mutex);
	d->monitor->stop = true;
	pthread_mutex_unlock(&d->monitor->monitor_mutex);
	wake_all_dongles(d);
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
		{
			pthread_mutex_unlock(&d->coders[i].coder_mutex);
			i++;
			continue ;
		}
		diff = now - d->coders[i].last_compile_start;
		pthread_mutex_unlock(&d->coders[i].coder_mutex);
		if (diff >= d->config->time_to_burnout)
			return (i);
		i++;
	}
	return (-1);
}

void	*monitor_routine(void *arg)
{
	t_data		*d;
	int			dead;
	uint64_t	now;

	d = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&d->monitor->monitor_mutex);
		if (d->monitor->stop)
		{
			pthread_mutex_unlock(&d->monitor->monitor_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&d->monitor->monitor_mutex);
		now = get_time_ms() - d->monitor->start_time;
		dead = find_next_burnout(d, now);
		if (dead >= 0)
		{
			handle_dead(d, dead, now);
			return (NULL);
		}
		usleep(1000);
	}
}
