/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routime.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 16:26:12 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/11 16:26:31 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_data *d, uint64_t now)
{
	uint32_t	i;
	uint64_t	diff;

	i = 0;
	while (i < d->config->num_coders)
	{
		pthread_mutex_lock(&d->monitor->mutex);
		if (d->coders[i].last_compile_start != 0)
		{
			diff = now - d->coders[i].last_compile_start;
			if (diff > d->config->time_to_burnout)
			{
				pthread_mutex_unlock(&d->monitor->mutex);
				return (i);
			}
		}
		pthread_mutex_unlock(&d->monitor->mutex);
		i++;
	}
	return (-1);
}

void	*monitor_routime(void *arg)
{
	t_data		*d;
	int			dead;
	uint64_t	now;

	d = (t_data *)arg;
	while (!d->monitor->stop)
	{
		now = get_time_ms() - d->monitor->start_time;
		dead = check_burnout(d, now);
		if (dead >= 0)
		{
			pthread_mutex_lock(&d->monitor->log_mutex);
			printf("%lu %d burned out\n", now,
				d->coders[dead].id);
			pthread_mutex_unlock(&d->monitor->log_mutex);
			pthread_mutex_lock(&d->monitor->mutex);
			d->monitor->stop = true;
			pthread_mutex_unlock(&d->monitor->mutex);
			pthread_mutex_lock(&d->monitor->dongle_mutex);
			pthread_cond_broadcast(&d->monitor->dongle_cond);
			pthread_mutex_unlock(&d->monitor->dongle_mutex);
			return (NULL);
		}
	}
	return (NULL);
}
