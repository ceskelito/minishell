/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:14 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/15 11:51:27 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "helpers.h"

static void	execute_in_child(t_cmd *cmd, pid_t *pid,
	int (*exec_cmd)(const char *, char *const[], char *const[]))
{
	bool	location_was_given;
	int		exit_code;

	*pid = fork();
	if (*pid == 0)
	{
		set_signal(SIGINT, handle_sigint);
		close_pipe_fds(cmd->next, PIPE | HEREDOC, -1);
		if (apply_redirs(cmd->redirs) != 0)
			return (ezg_cleanup(), exit(1));
		location_was_given = resolve_command_location(cmd);
		exit_code = try_execute_command(cmd, location_was_given, exec_cmd);
		return (ezg_cleanup(), exit(exit_code));
	}
	else if (*pid > 0)
		close_pipe_fds(cmd, PIPE | HEREDOC, 1);
	else
	{
		perror("minishell");
		set_exit_status(1);
	}
}

static int	execute_builtin(const char *pathname, char *const argv[],
		char *const envp[])
{
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
	if (ft_strcmp(pathname, "child") == 0)
		exit(get_exit_status());
	return (get_exit_status());
}

static void	execute_in_parent(t_shell *shell, t_cmd *cmd)
{
	if (apply_redirs(cmd->redirs) == 0)
		execute_builtin("parent", cmd->args, NULL);
	else
		set_exit_status(1);
	dup2(shell->std_in, STDIN_FILENO);
	dup2(shell->std_out, STDOUT_FILENO);
	ezg_group_release(EXECUTING);
}

// I'm afraid about put more instructions into a void return...
// but the norme push everybody to do this kind of orrible things
static void	execute_pipeline(t_cmd *cmd, int *exit_code)
{
	int		num_cmds;
	int		i;
	int		status;
	pid_t	*pid;
	bool	received_sigint;

	ezg_group_create("pid");
	num_cmds = count_cmds(cmd);
	pid = ezg_alloc("pid", sizeof(pid_t) * num_cmds);
	i = 0;
	set_signal(SIGINT, SIG_IGN);
	while (cmd)
	{
		if (is_builtin(cmd->args[0]))
			execute_in_child(cmd, &pid[i++], execute_builtin);
		else
			execute_in_child(cmd, &pid[i++], execve);
		ezg_group_release(EXECUTING);
		cmd = cmd->next;
	}
	received_sigint = wait_childrens(pid, &status, num_cmds);
	*exit_code = get_exit_code_from_status(status);
	if (received_sigint)
		write(STDOUT_FILENO, "\n", 1);
	return (ezg_group_release("pid"), set_signal(SIGINT, handle_sigint));
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
