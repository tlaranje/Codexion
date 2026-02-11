/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:29:58 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/11 17:12:10 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routime(void *arg)
{
	t_thread_args	*ta;

	ta = (t_thread_args *)arg;
	while (!ta->monitor->stop)
	{
		take_two_dongles(ta->coder, ta->monitor);
		pthread_mutex_lock(&ta->monitor->mutex);
		ta->coder->last_compile_start = get_time_ms() - ta->monitor->start_time;
		pthread_mutex_unlock(&ta->monitor->mutex);
		do_action(ta, "compiling", ta->config->time_to_compile);
		free_two_dongles(ta->coder, ta->monitor);
		do_action(ta, "debugging", ta->config->time_to_debug);
		do_action(ta, "refactoring", ta->config->time_to_refactor);
	}
	return (NULL);
}

void	start_threads(t_data *d)
{
	uint32_t	i;

	i = -1;
	while (++i < d->config->num_coders)
	{
		init_thread_args(i, d);
		pthread_create(&d->coders[i].thread, NULL, coder_routime, &d->args[i]);
	}
	usleep(1000);
}

void	join_threads(t_data *d)
{
	uint32_t	i;

	i = -1;
	while (++i < d->config->num_coders)
		pthread_join(d->coders[i].thread, NULL);
	pthread_join(d->monitor->thread, NULL);
}
