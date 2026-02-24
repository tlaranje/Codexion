/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:49:00 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/24 10:45:55 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

// Includes
# include <stdint.h>

// Forward declarations
typedef struct s_config			t_config;
typedef struct s_monitor		t_monitor;
typedef struct s_coder			t_coder;
typedef struct s_dongle			t_dongle;
typedef struct s_thread_args	t_thread_args;
typedef struct s_heap			t_heap;

// Structs
struct s_config
{
	uint32_t	num_coders;
	uint64_t	time_to_burnout;
	uint64_t	time_to_compile;
	uint64_t	time_to_debug;
	uint64_t	time_to_refactor;
	uint64_t	num_compiles;
	uint32_t	dongle_cooldown;
	char		*scheduler;
};

struct s_data
{
	t_config		*config;
	t_monitor		*monitor;
	t_coder			*coders;
	t_dongle		*dongles;
	t_heap			*heap;
	t_thread_args	*args;
};

#endif
