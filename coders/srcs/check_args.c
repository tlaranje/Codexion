/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 12:58:16 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/11 17:19:37 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_uint(const char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	/* if (index != 7 && atoi(str) == 0)
		return (0); */
	return (1);
}

static int	is_valid_scheduler(const char *s)
{
	return (strcmp(s, "fifo") == 0 || strcmp(s, "edf") == 0);
}

static int	check_mandatory_args(int argc)
{
	if (argc != 9)
	{
		fprintf(stderr, "Error: Missing mandatory arguments.\n\n"
			"Usage:\n./codexion "
			"number_of_coders time_to_burnout time_to_compile time_to_debug "
			"time_to_refactor number_of_compiles_required dongle_cooldown "
			"scheduler\n");
		return (1);
	}
	return (0);
}

int	check_args(int argc, const char *argv[])
{
	int	i;

	if (check_mandatory_args(argc) != 0)
		return (1);
	i = 1;
	while (i < argc)
	{
		if (i < 8)
		{
			if (!is_uint(argv[i]))
			{
				fprintf(stderr,
					"Error: Argument %d is invalid: '%s'\nArgument need to be "
					"a positive or valid int with only digits.\n", i, argv[i]);
				return (1);
			}
		}
		if (i == 8 && !is_valid_scheduler(argv[i]))
		{
			fprintf(stderr, "Error: scheduler must be fifo or edf\n");
			return (1);
		}
		i++;
	}
	return (0);
}
