#include "executor.h"
#include "ft_dprintf.h"
#include "ft_lib.h"
#include "ft_printf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int		count;
	char	*dir;

	count = 0;
	while(args[count])
		count++;
	if (count > 2)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return ;
	}
	if (count == 1)
	{
		dir = getenv("HOME");
		if (!dir)
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n");
			return ;
		}
	}
	else
		dir = args[1];
	if (strcmp(dir, "") != 0)
		if (chdir(dir) != 0)
			ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", dir, strerror(errno));
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
