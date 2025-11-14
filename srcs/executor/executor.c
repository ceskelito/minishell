/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:14 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/13 18:36:17 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <fcntl.h>

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

int	ft_strcmp(const char *s1, const char *s2)
{
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

static int	execute_builtin(const char *pathname, char * const argv[], char *const envp[])
{
	int	exit_value;
	
	(void)envp;
	if (!ft_strcmp(argv[0], "echo"))
		echo(argv);
	else if (!ft_strcmp(argv[0], "cd"))
		cd(argv);
	else if (!ft_strcmp(argv[0], "pwd"))
		pwd();
	else if (!ft_strcmp(argv[0], "env"))
		env();
	else if (!ft_strcmp(argv[0], "exit"))
		exit_shell(argv);
	else if (!ft_strcmp(argv[0], "export"))
		export(argv);
	else if (!ft_strcmp(argv[0], "unset"))
		unset(argv);	
	else
		exit_value = 1;
	exit_value = 0;
	if (ft_strcmp(pathname, "child") == 0)
		exit(exit_value);
	return (exit_value);
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
		else if (curr->type & HEREDOC)
			fd = curr->pipe_fd;
		else if (curr->type == APPEND)
			fd = open(curr->file, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644);
		else if (curr->type == OUT)
			fd = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
		else if (curr->type == IN)
			fd = open(curr->file, O_RDONLY | O_CLOEXEC);
		if (fd == -1)
		{
			print_error(curr->file, strerror(errno));
			return (errno);
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

static void	execute_in_child(t_cmd *cmd, pid_t *pid,
           	                     int (*exec_cmd)(const char *, char *const [], char *const []))
{
	// pid_t	pid;
	bool	is_child;

	*pid = fork();
	is_child = !(*pid);
	if (is_child)
	{
		close_pipe_fds(cmd->next, PIPE | HEREDOC, -1);
		if (apply_redirs(cmd->redirs) != 0)
			exit(errno);
		if (resolve_command_location(cmd), cmd->location)
			exec_cmd(cmd->location, cmd->args, ft_getenv_array());
		print_error(cmd->args[0], strerror(errno));
		exit(127);
	}
	else if (*pid > 0)
	{	
		// *saved_pid = pid;
		close_pipe_fds(cmd, PIPE | HEREDOC, 1);
	}
	else
	{
		perror("minishell");
		set_exit_status(errno);
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

int executor(t_shell *shell)
{
	t_cmd	*cmd;
	int		num_cmds;
	int		i;
	pid_t	*pid;
	int		exit_status;

	if (!shell || !shell->cmd_list)
		return (1);
	cmd = shell->cmd_list;
	setup_heredocs(cmd);
	if (!cmd->pipe_output && is_builtin(cmd->args[0]))
	{
		if (apply_redirs(cmd->redirs) != 0)
			return (errno);
		exit_status = execute_builtin("parent", cmd->args, NULL);
		reset_redirs(shell->std_in, shell->std_out);
		set_exit_status(exit_status);
		return (exit_status);
	}
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
		waitpid(pid[i++], &exit_status, 0);
	set_exit_status(exit_status);
	return (exit_status);
}

/*
int	old_executor(t_shell *shell)
{
	t_cmd	*cmd;
	int		num_cmds;
	int		i;
	pid_t	*pid;
	int		exit_status;

	if (!shell || !shell->cmd_list)
		return (1);
	cmd = shell->cmd_list;
	setup_heredocs(cmd);
	if (!cmd->pipe_output)
	{
		if (is_builtin(cmd->args[0]))
		{
			if (apply_redirs(cmd->redirs) != 0)
				return (errno);
			exit_status = execute_builtin("parent", cmd->args, NULL);
			reset_redirs(shell->std_in, shell->std_out);
		}
		else
		{
			pid = malloc(sizeof(pid_t));
			execute_in_child(cmd, pid, execve);
			waitpid(*pid, &exit_status, 0);
			free(pid);
		}
		ezg_group_release(EXECUTING);
	}
	else
	{
		num_cmds = count_cmds(cmd);
		pid = malloc(sizeof(pid_t) * num_cmds);
		open_pipeline_fds(cmd);
		i = 0;
		while (i < num_cmds)
		{
			if (is_builtin(cmd->args[0]))
				execute_in_child(cmd, &pid[i], execute_builtin);
			else
				execute_in_child(cmd, &pid[i], execve);
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
		free(pid);
	}
	set_exit_status(exit_status);
	return (exit_status);
}
*/
