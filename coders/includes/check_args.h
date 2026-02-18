/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlaranje <tlaranje@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:49:00 by tlaranje          #+#    #+#             */
/*   Updated: 2026/02/18 13:42:39 by tlaranje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECK_ARGS_H
# define CHECK_ARGS_H

// check_args.c
int	check_args(int argc, const char *argv[]);

// check_args_utils.c
int	is_uint(const char *str);
int	is_valid_scheduler(const char *s);

#endif