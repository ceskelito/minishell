#include "minishell.h"
#include "executor.h"
#include <stdbool.h>

// Need to expand variables
// Need to manage cases with EOF in quotes

int	setup_heredoc(char *delimiter)
{
	int		fd[2];
	char	*input;

	if (pipe(fd) == -1)
	{
		perror("minishell: pipe: ");
		return (-1);
	}
	input = NULL;
	while (true)
	{
		input = readline("> ");
		if (!input)
		{
			perror("minishell: warning: heredoc terminated by EOF\n");
			break;
		}
		ezg_add(EXECUTING, input);
		if (!ft_strcmp(input, delimiter))
			break;
		ft_dprintf(fd[1], "%s\n", input);
	}
	close(fd[1]);
	return (fd[0]);
}
