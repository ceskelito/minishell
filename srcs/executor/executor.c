#include "ft_dprintf.h"
#include "ft_printf.h"
#include "minishell.h"
#include "executor.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

/* BUILTIN's TO INCLUDE
 ◦ |DID| echo - with option -n
 ◦ |DID| cd - with only a relative or absolute path
 ◦ |DID| pwd - with no options
 ◦ export - with no options
 ◦ unset - with no options
 ◦ |DID| env - with no options or arguments
 ◦ |DID| exit - with no options
*/

int ft_strcmp(const char *s1, const char *s2)
{
    // Gestione dei casi NULL
    if (s1 == NULL && s2 == NULL)
        return 0; // entrambe le stringhe sono NULL → uguali
    if (s1 == NULL)
        return -1; // NULL considerata "minore" di una stringa valida
    if (s2 == NULL)
        return 1;  // stringa valida è "maggiore" di NULL

    // Confronto effettivo dei caratteri
    while (*s1 && *s2)
    {
        if (*s1 != *s2)
            return (unsigned char)*s1 - (unsigned char)*s2;
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

static bool	execute_builtin(char **args)
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
	else
		return (false);
	return (true);

}

static int redir_fd(t_redir *redirs)
{
    t_redir *curr;
    int fd;

    curr = redirs;
    while (curr)
    {
    	fd = -1;
    	if (curr->type & PIPE)
    		fd = curr->pipe_fd;
    	else if (curr->type & HEREDOC)
            fd = setup_heredoc(curr->file);
    	else if (curr->type & IN)
            fd = open(curr->file, O_RDONLY);
    	else if (curr->type & APPEND)
            fd = open(curr->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    	else if (curr->type & OUT)
            fd = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", curr->file, strerror(errno));
            curr = curr->next;
            return (1);
        }
        if (curr->type & (IN))
            dup2(fd, STDIN_FILENO);
        else
            dup2(fd, STDOUT_FILENO);
        close(fd);
        curr = curr->next;
    }
    return (0);
}

static void execute_cmd(char *location, char **args, char **env)
{
	pid_t		pid;
	int 		status;

	pid = fork();
	if (pid == 0)
	{
	    execve(location, args, env);
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", args[0], strerror(errno));
	    exit(127);
	}
	else if (pid > 0)
	{
	    waitpid(pid, &status, 0);
	}
	else
	{
	    perror("fork");
	}	
}

static inline void	reset_fd(int std_in, int std_out)
{
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
}

int executor(t_shell *shell)
{
	t_cmd		*cmd;
	extern char	**environ;

	cmd = shell->cmd_list;
	while (cmd)
	{
		setup_pipe(cmd);
		if (redir_fd(cmd->redirs) != 0)
			break;
		if (execute_builtin(cmd->args))
			;
		else
		{
			resolve_command_path(cmd);
			if (!cmd->location)
				ft_dprintf(STDERR_FILENO, "minishell: %s: Command not found\n", cmd->args[0]);
			else
				execute_cmd(cmd->location, cmd->args, environ);
		}
		reset_fd(shell->std_in, shell->std_out);
		ezg_group_release(EXECUTING);
		cmd = cmd->next;
	}
	return (0);
}

