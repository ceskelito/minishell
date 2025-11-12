/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:14 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/12 17:00:15 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezgalloc.h"
#include "ft_dprintf.h"
#include "ft_printf.h"
#include "minishell.h"
#include "executor.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	#include <stdlib.h>
	
	if (s1 == NULL && s2 == NULL)
		return (0);
	if (s1 == NULL)
		return (-1);
	if (s2 == NULL)
		return (1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

static bool	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset"));
}

static int	execute_builtin(char **args)
{
	if (!ft_strcmp(args[0], "echo"))
		echo(args);
	else if (!ft_strcmp(args[0], "cd"))
		cd(args);
	else if (!ft_strcmp(args[0], "pwd"))
		pwd();
	else if (!ft_strcmp(args[0], "env"))
		env();
	else if (!ft_strcmp(args[0], "exit"))
		exit_shell(args);
	else if (!ft_strcmp(args[0], "export"))
		export(args);
	else if (!ft_strcmp(args[0], "unset"))
		unset(args);	
	else
		return (1);
	return (0);
}

static int	apply_redirs(t_redir *redirs)
{
	t_redir	*curr;
	int		fd;

	curr = redirs;
	while (curr)
	{
		fd = -1;
		if (curr->type & PIPE)
			fd = curr->pipe_fd;
		else if (curr->type == HEREDOC)
			fd = setup_heredoc(curr->file);
		else if (curr->type == APPEND)
			fd = open(curr->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (curr->type == OUT)
			fd = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (curr->type == IN)
			fd = open(curr->file, O_RDONLY);
		if (fd == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", curr->file,
				strerror(errno));
			set_exit_status(1);
			curr = curr->next;
			return (1);
		}
		if (curr->type & IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		curr = curr->next;
	}
	return (0);
}

static int	execute_cmd_in_child(t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		apply_redirs(cmd->redirs);
		if (is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd->args);
			exit(get_exit_status());
		}
		else if (resolve_command_location(cmd), cmd->location)
		{
			execve(cmd->location, cmd->args, ft_getenv_array());
			print_error(cmd->args[0], strerror(errno));
			exit(127);
		}
	}
	else if (pid > 0)
	{
		while(cmd->redirs && (cmd->redirs->type & PIPE))
		{
			close(cmd->redirs->pipe_fd);
			cmd->redirs = cmd->redirs->next;
		}
	}
	else
	{
		perror("minishell");
		set_exit_status(errno);
	}
	return (pid);
}

static inline void	reset_fd(int std_in, int std_out)
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

int	executor(t_shell *shell)
{
	t_cmd	*cmd;
	int		num_cmds;
	int		i;
	pid_t	*pid;
	int		exit_status;

	if (!shell || !shell->cmd_list)
		return (1);
	cmd = shell->cmd_list;
	if (!cmd->pipe_output)
	{
		if (is_builtin(cmd->args[0]))
		{
			apply_redirs(cmd->redirs);
			exit_status = execute_builtin(cmd->args);
			reset_fd(shell->std_in, shell->std_out);
		}
		else
		{
			//pid[0] = execute_cmd_in_child(cmd);
			waitpid(execute_cmd_in_child(cmd), &exit_status, 0);
		}
		ezg_group_release(EXECUTING);
	}
	else
	{
		num_cmds = count_cmds(shell->cmd_list);
		ezg_group_create("pid");
		pid = ezg_alloc("pid", sizeof(pid_t) * num_cmds);
		setup_pipeline(shell->cmd_list);
		i = 0;
		while (i < num_cmds)
		{
			pid[i] = execute_cmd_in_child(cmd);
			cmd = cmd->next;
			ezg_group_release(EXECUTING);
			i++;
		}
		i = 0;
		while(i < num_cmds)
		{
			waitpid(pid[i], &exit_status, 0);
			i++;
		}
		ezg_group_release("pid");
	}
	set_exit_status(exit_status);
	return (exit_status);
}

