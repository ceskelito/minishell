/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:14 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/14 16:34:07 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

#ifndef O_CLOEXEC
# define O_CLOEXEC 0
#endif

static int	count_cmds(t_cmd *cmd_list)
{
	int	count;

	count = 0;
	while (cmd_list)
	{
		count++;
		cmd_list = cmd_list->next;
	}
	return (count);
}

void	executor(t_shell *shell)
{
	t_cmd	*cmd;
	pid_t	*pid;
	int		num_cmds;
	int		exit_code;
	int		i;

	if (!shell || !shell->cmd_list)
		return ;
	cmd = shell->cmd_list;
	setup_heredocs(cmd);
	if (is_builtin(cmd->args[0]) && !cmd->pipe_output)
	{
		execute_in_parent(shell, cmd);
		return ;
	}
	open_pipeline_fds(cmd);
	num_cmds = count_cmds(cmd);
	pid = malloc(sizeof(pid_t) * num_cmds);
	i = 0;
	while (cmd)
	{
		if (is_builtin(cmd->args[0]))
			execute_in_child(cmd, &pid[i++], execute_builtin);
		else
			execute_in_child(cmd, &pid[i++], execve);
		ezg_group_release(EXECUTING);
		cmd = cmd->next;
	}
	i = 0;
	while (i < num_cmds)
		waitpid(pid[i++], &exit_code, 0);
	set_exit_status(exit_code);
	free(pid);
}
