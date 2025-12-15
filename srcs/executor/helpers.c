/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:21:45 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/15 11:43:48 by rceschel         ###   ########.fr       */
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

int	try_execute_command(t_cmd *cmd, bool location_was_given,
	int (*exec_cmd)(const char *, char *const[], char *const[]))
{
	int	exit_code;

	exit_code = 0;
	if (cmd->location)
		exec_cmd(cmd->location, cmd->args, ft_getenv_array());
	exit_code = 127;
	if (location_was_given)
	{
		print_error(cmd->args[0], strerror(errno));
		if (errno == EACCES || errno == EISDIR)
			exit_code = 126;
	}
	else
		print_error(cmd->args[0], "command not found");
	return (exit_code);
}

bool	wait_childrens(int pid[], int *status, int num_of_cmds)
{
	int		i;
	bool	received_sigint;

	i = 0;
	received_sigint = false;
	while (i < num_of_cmds)
	{
		waitpid(pid[i++], status, 0);
		if (WIFSIGNALED(*status) && WTERMSIG(*status) == SIGINT)
			received_sigint = true;
	}
	return (received_sigint);
}
