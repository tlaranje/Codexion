/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:32:51 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/09 17:24:35 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_cd(t_config *config, t_coder coders[], t_dongle dongles[])
{
	uint32_t i;

	i = -1;
	while (++i < config->num_of_coders)
	{
		coders[i].config = config;
		coders[i].id = i + 1;

		dongles[i].cooldown = config->dongle_cooldown;
		dongles[i].id = i + 1;
		dongles[i].is_in_use = false;
		coders[i].left_dongle = &dongles[i];
		coders[i].right_dongle = &dongles[(i + 1) % config->num_of_coders];

	}
	for (i = 0; i < config->num_of_coders; i++)
	{
		printf("Coder[%d] - Left Dongle[%d]",coders[i].id, coders[i].left_dongle->id);
		printf(" - Right Dongle[%d]\n", coders[i].right_dongle->id);
	}

	return (0);
}


// D1 C1