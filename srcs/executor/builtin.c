#include "executor.h"
#include "ft_printf.h"

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

