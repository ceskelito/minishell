/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:16:30 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/14 14:21:19 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell"

void	execute_in_child(t_cmd *cmd, pid_t *pid,
           	    int (*exec_cmd)(const char *, char *const [], char *const []))
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

int     execute_builtin(const char *pathname, char * const argv[], char *const envp[])
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

void    execute_in_parent(t_shell *shell, t_cmd *cmd)
{
    if (apply_redirs(cmd->redirs))
		exit_code = execute_builtin("parent", cmd->args, NULL);
	else
		exit_code = errno;
	reset_redirs(shell->std_in, shell->std_out);
	set_exit_status(exit_code);
	ezg_group_release(EXECUTING);
}