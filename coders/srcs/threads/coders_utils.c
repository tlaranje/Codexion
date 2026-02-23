/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:29:58 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/23 16:30:31 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"
#include "codexion.h"
#include "utils.h"

static int	wait_dongle_cooldown(t_thread_args *ta, t_dongle *d)
{
	uint64_t		now;
	uint64_t		wait_ms;
	struct timespec	ts;

	now = get_time_ms() - ta->monitor->start_time;
	while (d->available_at > now)
	{
		wait_ms = d->available_at - now;
		set_timespec_delay(&ts, wait_ms);
		pthread_mutex_lock(&ta->monitor->monitor_mutex);
		if (ta->monitor->stop)
		{
			pthread_mutex_unlock(&ta->monitor->monitor_mutex);
			return (-1);
		}
		pthread_mutex_unlock(&ta->monitor->monitor_mutex);
		pthread_cond_timedwait(&d->dongle_cond, &d->dongle_mutex, &ts);
		now = get_time_ms() - ta->monitor->start_time;
	}
	return (0);
}

static int	take_dongle(t_thread_args *ta, t_dongle *dongle)
{
	uint64_t	now;

	pthread_mutex_lock(&dongle->dongle_mutex);
	while (1)
	{
		now = get_time_ms() - ta->monitor->start_time;
		if (!dongle->in_use && now >= dongle->available_at)
			break ;
		if (wait_dongle_cooldown(ta, dongle) == -1)
		{
			pthread_mutex_unlock(&dongle->dongle_mutex);
			return (-1);
		}
		if (dongle->in_use)
			pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
	}
	dongle->in_use = true;
	pthread_mutex_unlock(&dongle->dongle_mutex);
	return (0);
}

static int	wait_and_take_dongles(t_thread_args *ta, t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (take_dongle(ta, first) == -1)
		return (-1);
	if (take_dongle(ta, second) == -1)
	{
		pthread_mutex_lock(&first->dongle_mutex);
		first->in_use = false;
		pthread_cond_broadcast(&first->dongle_cond);
		pthread_mutex_unlock(&first->dongle_mutex);
		return (-1);
	}
	return (0);
}

int	take_two_dongles(t_thread_args *ta, t_coder *coder)
{
	if (wait_and_take_dongles(ta, coder) == -1)
		return (-1);
	pthread_mutex_lock(&ta->monitor->monitor_mutex);
	if (ta->monitor->stop)
	{
		pthread_mutex_unlock(&ta->monitor->monitor_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&ta->monitor->monitor_mutex);
	pthread_mutex_lock(&ta->monitor->log_mutex);
	printf("%lu %d has taken a dongle\n",
		get_time_ms() - ta->monitor->start_time, coder->id);
	printf("%lu %d has taken a dongle\n",
		get_time_ms() - ta->monitor->start_time, coder->id);
	pthread_mutex_unlock(&ta->monitor->log_mutex);
	return (0);
}

int	free_two_dongles(t_thread_args *ta, t_coder *c)
{
	uint64_t	now;

	now = get_time_ms() - ta->monitor->start_time;
	pthread_mutex_lock(&c->left_dongle->dongle_mutex);
	c->left_dongle->in_use = false;
	c->left_dongle->available_at = now + ta->monitor->config->dongle_cooldown;
	pthread_cond_broadcast(&c->left_dongle->dongle_cond);
	pthread_mutex_unlock(&c->left_dongle->dongle_mutex);
	pthread_mutex_lock(&c->right_dongle->dongle_mutex);
	c->right_dongle->in_use = false;
	c->right_dongle->available_at = now + ta->monitor->config->dongle_cooldown;
	pthread_cond_broadcast(&c->right_dongle->dongle_cond);
	pthread_mutex_unlock(&c->right_dongle->dongle_mutex);
	return (0);
}
