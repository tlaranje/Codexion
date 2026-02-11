/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utlis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:29:58 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/11 17:10:58 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_two_dongles(t_coder *c, t_monitor *m)
{
	uint64_t	now;

	pthread_mutex_lock(&m->dongle_mutex);
	while (c->left_dongle->is_in_use || c->right_dongle->is_in_use)
	{
		pthread_mutex_lock(&m->mutex);
		if (m->stop)
		{
			pthread_mutex_unlock(&m->mutex);
			pthread_mutex_unlock(&m->dongle_mutex);
			return (-1);
		}
		pthread_mutex_unlock(&m->mutex);
		pthread_cond_wait(&m->dongle_cond, &m->dongle_mutex);
	}
	c->left_dongle->is_in_use = true;
	c->right_dongle->is_in_use = true;
	pthread_mutex_unlock(&m->dongle_mutex);
	now = get_time_ms() - m->start_time;
	pthread_mutex_lock(&m->log_mutex);
	printf("%lu %d has taken a dongle\n", now, c->id);
	printf("%lu %d has taken a dongle\n", now, c->id);
	pthread_mutex_unlock(&m->log_mutex);
	return (0);
}

int	free_two_dongles(t_coder *c, t_monitor *m)
{
	pthread_mutex_lock(&m->dongle_mutex);
	c->left_dongle->is_in_use = false;
	c->right_dongle->is_in_use = false;
	pthread_cond_broadcast(&m->dongle_cond);
	pthread_mutex_unlock(&m->dongle_mutex);
	return (0);
}

void	do_action(t_thread_args *ta, const char *action, uint64_t duration)
{
	uint64_t	now;

	now = get_time_ms() - ta->monitor->start_time;
	pthread_mutex_lock(&ta->monitor->log_mutex);
	pthread_mutex_lock(&ta->monitor->mutex);
	if (!ta->monitor->stop)
		printf("%lu %d is %s\n", now, ta->coder->id, action);
	pthread_mutex_unlock(&ta->monitor->mutex);
	pthread_mutex_unlock(&ta->monitor->log_mutex);
	usleep(duration * 1000);
}
