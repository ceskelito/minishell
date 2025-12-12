/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:21:45 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/12 17:22:13 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_code_from_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (status);
}

void	try_execute_command(t_cmd *cmd, bool location_was_given,
	int *exit_code,
	int (*exec_cmd)(const char *, char *const[], char *const[]))
{
	if (cmd->location)
		exec_cmd(cmd->location, cmd->args, ft_getenv_array());
	*exit_code = 127;
	if (location_was_given)
	{
		print_error(cmd->args[0], strerror(errno));
		if (errno == EACCES || errno == EISDIR)
			*exit_code = 126;
	}
	else
		print_error(cmd->args[0], "command not found");
}
