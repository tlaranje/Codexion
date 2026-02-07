/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:45:44 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/07 01:03:16 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"



int	main(int argc, const char *argv[])
{
	struct args	args_data;

	if (check_args(argc, argv) == 0)
	{
		args_data = convert_args(argc, argv);
		printf("Num_coders: %d\n", args_data.num_coders);
		create_threads(argv);
	}

	return (0);
}
