/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:45:44 by tlaranje          #+#    #+#             */
/*   Updated: 2026/01/30 17:06:19 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, const char *argv[])
{
	if (check_args(argc, argv) == 0)
		printf("Done\n");
	return (0);
}
