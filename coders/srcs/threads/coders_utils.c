/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:29:58 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 16:37:54 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "min_heap.h"
#include "threads.h"
#include "codexion.h"
#include "utils.h"

static int	take_dongle(t_thread_args *ta, t_dongle *d)
{
	pthread_mutex_lock(&d->dongle_mutex);
	while (d->in_use && !ta->monitor->stop)
		pthread_cond_wait(&d->dongle_cond, &d->dongle_mutex);
	if (ta->monitor->stop)
		return (pthread_mutex_unlock(&d->dongle_mutex), -1);
	d->in_use = true;
	pthread_mutex_unlock(&d->dongle_mutex);
	return (0);
}

static int	wait_and_take_dongles(t_thread_args *ta, t_coder *c)
{
	if (take_dongle(ta, c->left_dongle) == -1)
		return (-1);
	if (take_dongle(ta, c->right_dongle) == -1)
	{
		pthread_mutex_lock(&c->left_dongle->dongle_mutex);
		c->left_dongle->in_use = false;
		pthread_cond_broadcast(&c->left_dongle->dongle_cond);
		pthread_mutex_unlock(&c->left_dongle->dongle_mutex);
		return (-1);
	}
	return (0);
}

int	take_two_dongles(t_thread_args *ta, t_coder *c)
{
	if (wait_and_take_dongles(ta, c) == -1)
	{
		pthread_mutex_unlock(&ta->dongle->dongle_mutex);
		return (-1);
	}
	pthread_mutex_lock(&ta->monitor->log_mutex);
	printf("%lu %d has taken a dongle\n",
		get_time_ms() - ta->monitor->start_time, c->id);
	printf("%lu %d has taken a dongle\n",
		get_time_ms() - ta->monitor->start_time, c->id);
	pthread_mutex_unlock(&ta->monitor->log_mutex);
	return (0);
}

int	free_two_dongles(t_thread_args *ta, t_coder *c)
{
	(void)ta;
	pthread_mutex_lock(&c->left_dongle->dongle_mutex);
	c->left_dongle->in_use = false;
	pthread_cond_broadcast(&c->left_dongle->dongle_cond);
	pthread_mutex_unlock(&c->left_dongle->dongle_mutex);

	pthread_mutex_lock(&c->right_dongle->dongle_mutex);
	c->right_dongle->in_use = false;
	pthread_cond_broadcast(&c->right_dongle->dongle_cond);
	pthread_mutex_unlock(&c->right_dongle->dongle_mutex);
	return (0);
}

void	do_action(t_thread_args *ta, const char *action, uint64_t duration)
{
	uint64_t	now;

	now = get_time_ms() - ta->monitor->start_time;
	pthread_mutex_lock(&ta->monitor->monitor_mutex);
	pthread_mutex_lock(&ta->monitor->log_mutex);
	if (strcmp(action, "compiling") == 0)
		ta->coder->compile_count++;
	if (!ta->monitor->stop)
		printf("%lu %d is %s\n", now, ta->coder->id, action);
	pthread_mutex_unlock(&ta->monitor->log_mutex);
	pthread_mutex_unlock(&ta->monitor->monitor_mutex);
	usleep(duration * 1000);
}
