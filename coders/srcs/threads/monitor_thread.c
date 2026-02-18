/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:03:28 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 16:43:08 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"
#include "codexion.h"
#include "utils.h"

static int	check_burnout(t_data *d, uint64_t now)
{
	uint32_t i;
	uint64_t last;
	uint64_t diff;

	i = 0;
	while (i < d->config->num_coders)
	{
		pthread_mutex_lock(&d->coders[i].coder_mutex);
		last = d->coders[i].last_compile_start;
		pthread_mutex_unlock(&d->coders[i].coder_mutex);
		if (last != 0)
		{
			diff = now - last;
			if (diff > d->config->time_to_burnout)
				return (i);
		}
		i++;
	}
	return (-1);
}

static void wake_all_dongles(t_data *d)
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
	d->monitor->stop = 1;
	pthread_mutex_unlock(&d->monitor->monitor_mutex);
	wake_all_dongles(d);
}

void	*monitor_routime(void *arg)
{
	t_data	*d = (t_data *)arg;
	int		dead;
	uint64_t	now;

	while (1)
	{
		pthread_mutex_lock(&d->monitor->monitor_mutex);
		if (d->monitor->stop)
		{
			pthread_mutex_unlock(&d->monitor->monitor_mutex);
			break ;
		}
		pthread_mutex_unlock(&d->monitor->monitor_mutex);
		now = get_time_ms() - d->monitor->start_time;
		if ((dead = check_burnout(d, now)) >= 0)
		{
			handle_dead(d, dead, now);
			return (NULL);
		}
	}
	return (NULL);
}
