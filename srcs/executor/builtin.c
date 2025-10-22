#include "executor.h"
#include "ft_lib.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <unistd.h>

void	env()
{
	extern const char	**environ;
	int					i;

	i = 0;
	while(environ[i])
	{
		ft_printf("%s\n", environ[i]);
		i++;
	}
	
}

void	echo(char **args) 
{
	int		i;
	int		j;
	bool	flag_n;

	i = 1;
	flag_n = false;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
    	while (args[i][j] == 'n')
			j++;
    	if (args[i][j] != '\0' || j == 1)
			break;
       flag_n = true;
       i++;
    }
	while(args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!flag_n)
		ft_printf("\n");
}

void	pwd()
{	
	ft_printf("%s\n", getcwd(NULL, 0));
}

void	cd(char **args)
{
	int	i;

	i = 0;
	while(args[i])
		i++;
	if (i > 2)
		perror("cd: too many arguments\0");
	else if (i == 1)
	{
		if (chdir(getenv("HOME")) != 0)
			perror("cd: HOME not set\0");
	}
	else if (strcmp(args[1], "") == 0)
		;	
	else if (chdir(args[1]) != 0)
		perror(ft_strjoin("cd: ", args[1]));
}

void	exit_shell(char **args)
{
	int	status;
	int	i;

	ft_printf("exit\n");

	if (!args[1])
		exit(EXIT_SUCCESS);
	
	i = 0;
	while (args[1][i])
	{
		if (!ft_isdigit(args[1][i]) && !(i == 0 && (args[1][i] == '+' || args[1][i] == '-')))
		{
			ft_dprintf(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n", args[1]);
			exit(255);
		}
		i++;
	}

	if (args[2])
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		//g_exit_status = 1; // Need to update exit status
		return; // Like Bash, do not exit from the shell
	}

	status = ft_atoi(args[1]) % 256;
	exit(status);
}
