/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:29:58 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/10 17:36:42 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routime(void *arg)
{
	t_thread_args	*ta;

	ta = (t_thread_args *)arg;
	while (!ta->monitor->stop)
	{
		take_dongle(ta->coder->left_dongle, ta->monitor, ta->coder);
		take_dongle(ta->coder->right_dongle, ta->monitor, ta->coder);
		compiling(ta);
		free_dongle(ta->coder->left_dongle);
		free_dongle(ta->coder->right_dongle);
		debugging(ta);
		refactoring(ta);
	}
	return (NULL);
}

void	start_threads(t_data *d)
{
	uint32_t	i;

	i = -1;
	while (++i < d->config->num_coders)
	{
		pthread_mutex_init(&d->dongles[i].dongle_mutex, NULL);
		pthread_cond_init(&d->dongles[i].cond, NULL);
		init_thread_args(i, d);
		pthread_create(&d->coders[i].thread, NULL, coder_routime, &d->args[i]);
		usleep(1000 * 1000);
	}
}

void	join_threads(t_data *d)
{
	uint32_t	i;

	i = -1;
	while (++i < d->config->num_coders)
		pthread_join(d->coders[i].thread, NULL);
}
