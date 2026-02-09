/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:45:44 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/09 16:42:11 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*test(void *arg)
{
	t_coder *coder = (t_coder *)arg;
	uint64_t time_now = get_time_ms();
	printf("%lu %d has taken a dongle\n",
		(time_now - coder->config->start_time), coder->id);
	return (NULL);
}

int	main(int argc, const char *argv[])
{
	t_config config;
	uint32_t		i;

	if (check_args(argc, argv) == 0)
		config = parse_args(argv);

	t_coder coders[config.num_of_coders];
	t_dongle dongles[config.num_of_coders];

	config.start_time = get_time_ms();

	init_cd(&config, coders, dongles);

	i = -1;
	while (++i < config.num_of_coders)
	{
		pthread_create(&coders[i].thread, NULL, test, &coders[i]);
		usleep(5 * 1000);
	}

	i = -1;
	while (++i < config.num_of_coders)
		pthread_join(coders[i].thread, NULL);
	return (0);
}
