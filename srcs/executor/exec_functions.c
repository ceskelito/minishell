/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:16:30 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/19 11:34:27 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

static void	close_pipe_fds(t_cmd *cmd, int redir_type, int cmd_to_parse)
{
	t_cmd	*curr_cmd;
	t_redir	*curr_redir;
	int		num_of_parsed;

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
		else if (curr->type & APPEND)
			fd = open(curr->file, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC,
					0644);
		else if (curr->type & OUT)
			fd = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC,
					0644);
		else if (curr->type & IN)
			fd = open(curr->file, O_RDONLY | O_CLOEXEC);
		if (fd == -1)
		{
			close(fd);
			return (print_error(curr->file, strerror(errno)), errno);
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

void	execute_in_child(t_cmd *cmd, pid_t *pid, int (*exec_cmd)(const char *,
			char *const[], char *const[]))
{
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
		close_pipe_fds(cmd, PIPE | HEREDOC, 1);
	else
	{
		perror("minishell");
		set_exit_status(errno);
	}
}

int	execute_builtin(const char *pathname, char *const argv[],
		char *const envp[])
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

void	execute_in_parent(t_shell *shell, t_cmd *cmd)
{
	int	exit_code;

	if (apply_redirs(cmd->redirs) == 0)
		exit_code = execute_builtin("parent", cmd->args, NULL);
	else
		exit_code = errno;
	dup2(shell->std_in, STDIN_FILENO);
	dup2(shell->std_out, STDOUT_FILENO);
	set_exit_status(exit_code);
	ezg_group_release(EXECUTING);
}
