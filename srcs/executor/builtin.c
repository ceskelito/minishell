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

void	echo(char *str, bool flag_n) 
{
	if (flag_n)
		ft_printf("%s", str);
	else
		ft_printf("%s\n", str);
}

void	print_workig_directory()
{	
	ft_printf("%s\n", getcwd(NULL, 0));
}

void	change_directory(char *dirname)
{
	// Need to manage the "too many arguments" case.
	// idk how to do it before the end of tokenizer
	//----
	// is also needed to manage the case in wich HOME env is not set
	if (!dirname)
	{
		chdir(getenv("HOME"));
		return;
	}
	if (strcmp(dirname, "") == 0)
		return;	
	if (chdir(dirname) != 0)
		perror(ft_strjoin("cd: ", dirname));
}

