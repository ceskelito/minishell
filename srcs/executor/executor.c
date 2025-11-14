/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:14 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/14 14:22:13 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

static int	apply_redirs(t_redir *redirs)
{
	t_redir	*curr;
	int		fd;
	
	curr = redirs;
	while (curr)
	{
		fd = -1;
		if (curr->type & (PIPE | HEREDOC))
			fd = curr->pipe_fd;
		else if (curr->type == APPEND)
			fd = open(curr->file, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644);
		else if (curr->type == OUT)
			fd = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
		else if (curr->type == IN)
			fd = open(curr->file, O_RDONLY | O_CLOEXEC);
		if (fd == -1)
			return (print_error(curr->file, strerror(errno)), errno);
		if (curr->type & IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		curr = curr->next;
	}
	return (0);
}

static void	close_pipe_fds(t_cmd *cmd, int redir_type, int cmd_to_parse)
{
	t_cmd	*curr_cmd;
	t_redir	*curr_redir;
	int 	num_of_parsed;

	num_of_parsed = 0;
	curr_cmd = cmd;
	while (curr_cmd && (cmd_to_parse == -1 || num_of_parsed < cmd_to_parse))
	{
		curr_redir = curr_cmd->redirs;
		while (curr_redir)
		{
			if (curr_redir->type & (redir_type))
				close(curr_redir->pipe_fd);
			curr_redir = curr_redir->next;
		}
		curr_cmd = curr_cmd->next;
		num_of_parsed++;
	}
	
}

static inline void	reset_redirs(int std_in, int std_out)
{
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
}

int	count_cmds(t_cmd *cmd_list)
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

void executor(t_shell *shell)
{
	t_cmd	*cmd;
	pid_t	*pid;
	int		num_cmds;
	int		exit_code;
	int		i;

	if (!shell || !shell->cmd_list)
		return (1);
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
