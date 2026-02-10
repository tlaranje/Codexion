/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:45:44 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/10 17:41:39 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routime(void *arg)
{
	t_data		*d;
	uint32_t	i;
	uint64_t	diff;
	uint64_t	now;

	d = (t_data *)arg;
	while (!d->monitor->stop)
	{
		now = get_time_ms();
		i = -1;
		while(++i < d->config->num_coders)
		{
			diff = now - d->coders[i].last_compile_start;
			if (diff > d->config->time_to_burnout)
			{
				pthread_mutex_lock(&d->monitor->log_mutex);
				printf("%lu %d burned out\n", now, d->coders[i].id);
				pthread_mutex_unlock(&d->monitor->log_mutex);
				d->monitor->stop = true;
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}

int	main(int argc, const char *argv[])
{
	t_data	d;

	if (check_args(argc, argv) == 0)
	{
		malloc_structs(&d, argc, argv);
		d.monitor->start_time = get_time_ms();

		init_coder_dongle(&d);
		d.monitor->coders = d.coders;
		pthread_mutex_init(&d.monitor->log_mutex, NULL);
		pthread_create(&d.monitor->thread, NULL, monitor_routime, &d);
		start_threads(&d);
		join_threads(&d);
	}
	return (0);
}
