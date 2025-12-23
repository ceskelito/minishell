/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 11:51:46 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/15 11:51:47 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_H
# define HELPERS_H

# include "minishell.h"

void	set_signal(int signum, void (*handler)(int));
void	handle_sigint(int signal);
int		get_exit_code_from_status(int status);
int		try_execute_command(t_cmd *cmd, bool location_was_given,
			int (*exec_cmd)(const char *, char *const[], char *const[]));
bool	wait_childrens(int pid[], int *status, int num_of_cmds);

#endif
