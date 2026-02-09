/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:49:00 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/09 16:08:45 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdint.h>
# include <stdbool.h>

typedef struct s_config
{
	uint32_t	num_of_coders;
	uint64_t	time_to_burnout;
	uint64_t	time_to_compile;
	uint64_t	time_to_debug;
	uint64_t	time_to_refactor;
	uint64_t	num_compiles;
	uint32_t	dongle_cooldown;
	uint64_t	start_time;
	char		*scheduler;
}	t_config;

typedef struct s_dongle
{
	uint32_t	id;
	uint32_t	cooldown;
	bool		is_in_use;

}	t_dongle;

typedef struct s_coder
{
	uint32_t	id;
	pthread_t	thread;
	t_config	*config;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
}	t_coder;


int			check_args(int argc, const char *argv[]);
t_config	parse_args(const char *argv[]);
int			init_cd(t_config *config, t_coder coders[], t_dongle dongles[]);
uint64_t	get_time_ms(void);

#endif
