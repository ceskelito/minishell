#include "minishell.h"
#include "executor.h"

/* BUILTIN's TO INCLUDE
 ◦ |DID| echo - with option -n
 ◦ |DID| cd - with only a relative or absolute path
 ◦ |DID| pwd - with no options
 ◦ export - with no options
 ◦ unset - with no options
 ◦ |DID| env - with no options or arguments
 ◦ exit - with no options
*/

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2) // finché nessuna delle due stringhe è terminata
    {
        if (*s1 != *s2)
            return (unsigned char)*s1 - (unsigned char)*s2;
        s1++;
        s2++;
    }
    // se siamo usciti dal ciclo, almeno una delle due è terminata
    return (unsigned char)*s1 - (unsigned char)*s2;
}

bool	execute_builtin(char **args)
{
	if (!ft_strcmp(args[0], "echo"))
		echo(args);
	else if (!ft_strcmp(args[0], "cd"))
		cd(args);
	else if (!ft_strcmp(args[0], "pwd"))
		pwd();
	else if (!ft_strcmp(args[0], "env"))
		env();
	else
		return (false);
	return (true);

}

void redir_fd(t_redir *redirs)
{
    t_redir *curr;
    int		fd;

	curr = redirs;
    while (curr)
    {
        if (curr->type & IN) {
            fd = open(curr->file, O_RDONLY);
            dup2(fd, STDIN_FILENO);
        }
        else if (curr->type & OUT) {
            fd = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC);
            dup2(fd, STDOUT_FILENO);
        }
        else if (curr->type & APPEND) {
            fd = open(curr->file, O_WRONLY | O_CREAT | O_APPEND);
            dup2(fd, STDOUT_FILENO);
        }
        else if (curr->type & HEREDOC) {
            // Heredoc implementation
            //setup_heredoc(curr->file);
        }
		close(fd);
        curr = curr->next;
    }
}
void	reset_fd(int std_in, int std_out)
{
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
}

void execute_cmd(char *location, char **args)
{
	extern char	**environ;
	pid_t		pid;
	int 		status;

	pid = fork();
	if (pid == 0)
	{
	    execve(location, args, environ);
	    perror(args[0]);
	    _exit(127);
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

//int executor(t_shell *shell)
//{
//	t_cmd		*curr;
//	char		*location;
//	extern char	**environ;
//	char		**args;
//	
//	curr = shell->cmd_list;
//	while (curr)
//	{
//		args = curr->args;
//		redir_fd(curr->redirs);
//		if (execute_builtin(args))
//		{
//			reset_fd(shell->std_in, shell->std_out);
//			curr = curr->next;
//			continue;
//		}
//		location = get_location(args[0]);
//		if (!location)
//		{
//			ft_dprintf(STDERR_FILENO, "%s: command not found\n\0", args[0]);
//			reset_fd(shell->std_in, shell->std_out);
//			curr = curr->next;
//			return (1);
//		}
//		execute_cmd(ft_strjoin(location, args[0]), args);
//		reset_fd(shell->std_in, shell->std_out);
//		curr = curr->next;
//		ezg_group_release(EXECUTING);
//	}
//	return (0);
//}


int executor(t_shell *shell)
{
	t_cmd		*curr;
	char		*location;
	extern char	**environ;
	char		**args;

	curr = shell->cmd_list;
	while (curr)
	{
		args = curr->args;
		redir_fd(curr->redirs);

		if (!execute_builtin(args))
		{
			location = get_location(args[0]);
			if (!location)
				ft_dprintf(STDERR_FILENO, "%s: command not found\n", args[0]);
			else
				execute_cmd(ft_strjoin(location, args[0]), args);
		}
		reset_fd(shell->std_in, shell->std_out);
		ezg_group_release(EXECUTING);
		curr = curr->next;
	}
	return (0);
}
