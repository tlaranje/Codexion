/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utlis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:29:58 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/16 17:08:32 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	wait_and_take_dongles(t_coder *c, t_monitor *m)
{
	t_coder	*next;

	while (!m->stop)
	{
		pthread_mutex_lock(&m->heap_mutex);
		next = heap_peek(&m->wait_heap);
		if (next == c)
		{
			pthread_mutex_unlock(&m->heap_mutex);
			break;
		}
		pthread_mutex_unlock(&m->heap_mutex);
		pthread_cond_wait(&m->dongle_cond, &m->dongle_mutex);
	}
	while (!m->stop && (c->left_dongle->in_use || c->right_dongle->in_use))
		pthread_cond_wait(&m->dongle_cond, &m->dongle_mutex);
	if (m->stop)
		return (-1);
	c->left_dongle->in_use = true;
	c->right_dongle->in_use = true;
	pthread_mutex_lock(&m->heap_mutex);
	heap_pop(&m->wait_heap, m->config->scheduler);
	pthread_mutex_unlock(&m->heap_mutex);
	return (0);
}

int	take_two_dongles(t_coder *c, t_monitor *m)
{
	pthread_mutex_lock(&m->dongle_mutex);
	if (wait_and_take_dongles(c, m) == -1)
	{
		pthread_mutex_unlock(&m->dongle_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&m->dongle_mutex);

	pthread_mutex_lock(&m->log_mutex);
	printf("%lu %d has taken a dongle\n", get_time_ms() - m->start_time, c->id);
	printf("%lu %d has taken a dongle\n", get_time_ms() - m->start_time, c->id);
	pthread_mutex_unlock(&m->log_mutex);

	return (0);
}

int	free_two_dongles(t_coder *c, t_monitor *m)
{
	pthread_mutex_lock(&m->dongle_mutex);
	c->left_dongle->in_use = false;
	c->right_dongle->in_use = false;
	usleep(m->config->dongle_cooldown * 1000);
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
	if (strcmp(action, "compiling") == 0)
		ta->coder->compile_count++;
	if (!ta->monitor->stop)
		printf("%lu %d is %s\n", now, ta->coder->id, action);
	pthread_mutex_unlock(&ta->monitor->mutex);
	pthread_mutex_unlock(&ta->monitor->log_mutex);
	usleep(duration * 1000);
}
