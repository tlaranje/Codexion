/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utlis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:29:58 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/10 17:36:20 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int take_dongle(t_dongle *d, t_monitor *m, t_coder *c)
{
	uint64_t now = get_time_ms();
	pthread_mutex_lock(&m->log_mutex);
	printf("%lu %d has taken a dongle\n",
		(now - m->start_time), c->id);
	pthread_mutex_unlock(&m->log_mutex);
	pthread_mutex_lock(&d->dongle_mutex);
	while (d->is_in_use)
		pthread_cond_wait(&d->cond, &d->dongle_mutex);
	d->is_in_use = true;
	pthread_mutex_unlock(&d->dongle_mutex);
	return (0);
}

int free_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->is_in_use = false;
	pthread_cond_signal(&dongle->cond);
	pthread_mutex_unlock(&dongle->dongle_mutex);
	return (0);
}

void	compiling(t_thread_args *ta)
{
	uint64_t	now;

	now = get_time_ms();
	ta->coder->last_compile_start = now;
	pthread_mutex_lock(&ta->monitor->log_mutex);
	printf("%lu %d is compiling\n",
		now - ta->monitor->start_time, ta->coder->id);
	pthread_mutex_unlock(&ta->monitor->log_mutex);
	usleep(ta->config->time_to_compile * 1000);
}

void	debugging(t_thread_args *ta)
{
	uint64_t now = get_time_ms();

	now = get_time_ms();
	pthread_mutex_lock(&ta->monitor->log_mutex);
	printf("%lu %d is debugging\n",
		now - ta->monitor->start_time, ta->coder->id);
	pthread_mutex_unlock(&ta->monitor->log_mutex);
	usleep(ta->config->time_to_debug * 1000);
}

void	refactoring(t_thread_args *ta)
{
	uint64_t now = get_time_ms();

	now = get_time_ms();
	pthread_mutex_lock(&ta->monitor->log_mutex);
	printf("%lu %d is refactoring\n",
		now - ta->monitor->start_time, ta->coder->id);
	pthread_mutex_unlock(&ta->monitor->log_mutex);
	usleep(ta->config->time_to_refactor * 1000);
}