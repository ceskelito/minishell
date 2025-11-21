/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:14 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/20 16:44:13 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

#ifndef O_CLOEXEC
# define O_CLOEXEC 0
#endif

static void	execute_in_child(t_cmd *cmd, pid_t *pid,
	int (*exec_cmd)(const char *, char *const[], char *const[]))
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

static int	execute_builtin(const char *pathname, char *const argv[],
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

static void	execute_in_parent(t_shell *shell, t_cmd *cmd)
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

static void	execute_pipeline(t_cmd *cmd, int *exit_code)
{
	int		num_cmds;
	int		i;
	pid_t	*pid;
	int		status;

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
	{
		waitpid(pid[i++], &status, 0);
		if (WIFEXITED(status))
			*exit_code = WEXITSTATUS(status);
	}
	free(pid);
}

void	executor(t_shell *shell)
{
	t_cmd	*cmd;
	int		exit_code;

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
	execute_pipeline(cmd, &exit_code);
	set_exit_status(exit_code);
}
