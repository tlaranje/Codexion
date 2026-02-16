/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:45:44 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/16 11:40:06 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

uint64_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	main(int argc, const char *argv[])
{
	t_data	d;

	if (check_args(argc, argv) == 0)
	{
		malloc_structs(&d, argc, argv);
		init_monitor_mutexes(d.monitor);
		d.monitor->config = d.config;
		d.monitor->start_time = get_time_ms();
		init_coder_dongle(&d);
		if (d.config->num_coders <= 1 || d.config->time_to_burnout == 0)
		{
			usleep(d.config->time_to_burnout * 1000);
			printf("%lu %d burned out\n",
				d.config->time_to_burnout, d.coders->id);
			return (0);
		}
		d.monitor->coders = d.coders;
		pthread_create(&d.monitor->thread, NULL, monitor_routime, &d);
		start_threads(&d);
		join_threads(&d);
		free_all(&d);
	}
	return (0);
}
