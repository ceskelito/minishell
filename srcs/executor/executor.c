/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:14 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/07 18:34:06 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"
#include "ft_printf.h"
#include "minishell.h"
#include "executor.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

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

static int	count_commands(t_cmd *cmd)
{
	int		count;
	t_cmd	*curr;

	count = 0;
	curr = cmd;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

static bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "env") ||
		!ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset"))
		return (true);
	return (false);
}

static void	execute_builtin(char **args)
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
}

static int	redir_fd(t_redir *redirs)
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

static void	close_all_pipes(int **pipes, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		if (pipes[i][0] != -1)
			close(pipes[i][0]);
		if (pipes[i][1] != -1)
			close(pipes[i][1]);
		i++;
	}
}

static void	reset_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	execute_single_cmd(t_cmd *cmd, char **env, int **pipes, int n_pipes)
{
	if (redir_fd(cmd->redirs) != 0)
		exit(1);
	close_all_pipes(pipes, n_pipes);
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd->args);
		exit(0);
	}
	resolve_command_path(cmd);
	if (!cmd->location)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: Command not found\n",
			cmd->args[0]);
		exit(127);
	}
	execve(cmd->location, cmd->args, env);
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd->args[0],
		strerror(errno));
	exit(127);
}

static int	wait_for_children(pid_t *pids, int n_cmds)
{
	int	i;
	int	status;
	int	exit_code;

	exit_code = 0;
	i = 0;
	while (i < n_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = 128 + WTERMSIG(status);
		i++;
	}
	return (exit_code);
}

static int	**setup_pipes_in_parent(t_cmd *cmd_list, int n_cmds)
{
	int		**pipes;
	int		i;
	int		n_pipes;
	t_cmd	*curr;

	n_pipes = n_cmds - 1;
	pipes = ezg_calloc(EXECUTING, sizeof(int *), n_pipes);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < n_pipes)
	{
		pipes[i] = ezg_calloc(EXECUTING, sizeof(int), 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("minishell");
			return (NULL);
		}
		i++;
	}
	curr = cmd_list;
	i = 0;
	while (curr)
	{
		if (i < n_pipes)
		{
			t_redir *out_redir = ezg_calloc(EXECUTING, sizeof(t_redir), 1);
			out_redir->type = PIPE | OUT;
			out_redir->pipe_fd = pipes[i][1];
			out_redir->file = NULL;
			add_redir(curr, out_redir);
		}
		if (i > 0)
		{
			t_redir *in_redir = ezg_calloc(EXECUTING, sizeof(t_redir), 1);
			in_redir->type = PIPE | IN;
			in_redir->pipe_fd = pipes[i - 1][0];
			in_redir->file = NULL;
			add_redir(curr, in_redir);
		}
		curr = curr->next;
		i++;
	}
	return (pipes);
}

static int	execute_pipeline(t_shell *shell)
{
	int		n_cmds;
	int		**pipes;
	pid_t	*pids;
	t_cmd	*curr;
	int		i;
	pid_t	pid;

	n_cmds = count_commands(shell->cmd_list);
	pipes = setup_pipes_in_parent(shell->cmd_list, n_cmds);
	if (!pipes && n_cmds > 1)
		return (1);
	pids = ezg_calloc(EXECUTING, sizeof(pid_t), n_cmds);
	if (!pids)
		return (1);
	curr = shell->cmd_list;
	i = 0;
	while (i < n_cmds)
	{
		pid = fork();
		if (pid == 0)
		{
			reset_signal_handlers();
			execute_single_cmd(curr, ft_getenv_array(), pipes, n_cmds - 1);
		}
		else if (pid > 0)
		{
			pids[i] = pid;
		}
		else
		{
			perror("minishell");
			return (1);
		}
		curr = curr->next;
		i++;
	}
	close_all_pipes(pipes, n_cmds - 1);
	shell->exit_status = wait_for_children(pids, n_cmds);
	set_exit_status(shell->exit_status);
	return (0);
}

static int	execute_single_builtin(t_shell *shell, t_cmd *cmd)
{
	int	saved_stdin;
	int	saved_stdout;

	(void)shell;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (redir_fd(cmd->redirs) != 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		set_exit_status(1);
		return (1);
	}
	execute_builtin(cmd->args);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	set_exit_status(0);
	return (0);
}

static int	execute_single_external(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;
	int		exit_code;

	pid = fork();
	if (pid == 0)
	{
		reset_signal_handlers();
		execute_single_cmd(cmd, ft_getenv_array(), NULL, 0);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_code = 128 + WTERMSIG(status);
		else
			exit_code = 1;
		set_exit_status(exit_code);
		shell->exit_status = exit_code;
	}
	else
	{
		perror("minishell");
		set_exit_status(1);
		return (1);
	}
	return (0);
}

int	executor(t_shell *shell)
{
	int		n_cmds;
	t_cmd	*cmd;

	if (!shell->cmd_list)
		return (0);
	n_cmds = count_commands(shell->cmd_list);
	if (n_cmds > 1)
		return (execute_pipeline(shell));
	cmd = shell->cmd_list;
	if (is_builtin(cmd->args[0]))
		return (execute_single_builtin(shell, cmd));
	else
		return (execute_single_external(shell, cmd));
}
