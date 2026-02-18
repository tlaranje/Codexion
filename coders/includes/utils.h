/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:49:00 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 17:33:35 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

// Includes
# include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

// Forward declarations
typedef struct s_thread_args t_thread_args;
typedef struct s_data t_data;
typedef struct s_config t_config;

// init_utils.c
int			init_coder_dongle(t_data *d);
int			init_mutexes(t_data *d);
int			init_thread_args(uint32_t i, t_data *d);
int			init_config(t_config *config, int argc, const char *argv[]);

// utils.c
int			malloc_structs(t_data *d, int argc, const char *argv[]);
int			free_all(t_data *d);
uint64_t	get_time_ms(void);

#endif