/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:45:44 by tlaranje          #+#    #+#             */
/*   Updated: 2026/03/06 10:30:03 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "check_args.h"
#include "codexion.h"
#include "min_heap.h"
#include "threads.h"
#include "utils.h"

static int	handle_single_coder(t_data *d)
{
	usleep(d->config->time_to_burnout * 1000);
	printf("%lu %d burned out\n",
		d->config->time_to_burnout, d->coders->id);
	return (0);
}

int	main(int argc, const char *argv[])
{
	t_data	d;

	if (check_args(argc, argv) != 0)
		return (0);
	if (malloc_structs(&d, argc, argv) != 0)
		return (1);
	if (init_monitor(&d) != 0)
		return (1);
	if (d.config->num_coders == 1 || d.config->time_to_burnout == 0)
		return (handle_single_coder(&d));
	start_threads(&d);
	join_threads(&d);
	free_all(&d);
	return (0);
}
