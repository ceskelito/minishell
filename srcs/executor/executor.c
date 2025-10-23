#include "ft_dprintf.h"
#include "ft_printf.h"
#include "minishell.h"
#include "executor.h"
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
        if (curr->type & IN)
        	if (curr->type & PIPE)
        		fd = curr->pipe_fd;
        	else
	            fd = open(curr->file, O_RDONLY);
        else if (curr->type & OUT)
        	if (curr->type & PIPE)
        		fd = curr->pipe_fd;
        	else
	            fd = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (curr->type & APPEND)
            fd = open(curr->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (curr->type & HEREDOC)
            fd = setup_heredoc(curr->file);
        if (fd == -1)
        {
            perror(ft_strjoin("minishell: ", curr->file));
            curr = curr->next;
            return (1);
        }
        if (curr->type & (IN | HEREDOC))
            dup2(fd, STDIN_FILENO);
        else
            dup2(fd, STDOUT_FILENO);
        close(fd);
        curr = curr->next;
    }
    return (0);
}

static void execute_cmd(char *location, char **args)
{
	extern char	**environ;
	pid_t		pid;
	int 		status;

	pid = fork();
	if (pid == 0)
	{
	    execve(location, args, environ);
	    perror(args[0]);
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
	t_cmd		*curr;
	char		*location;
	extern char	**environ;
	char		**args;

	curr = shell->cmd_list;
	while (curr)
	{
		if (redir_fd(curr->redirs) != 0)
			break;
		args = curr->args;
		if (execute_builtin(args))
			;
		else
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

