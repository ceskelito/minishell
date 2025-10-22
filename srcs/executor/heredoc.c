#include "ft_dprintf.h"
#include "ft_lib.h"
#include "minishell.h"
#include "executor.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

// static int	create_buffer()
// {
// 	int		i;
// 	int		fd;
// 	char	*file;
	
// 	i = 0;
// 	fd = -1;
// 	while(fd == -1)
// 	{
// 		file = ft_strjoin("mini_heredoc_", ft_itoa(i));
// 		ezg_add(EXECUTING, file);
// 		fd = open(file, O_CREAT | O_EXCL | O_RDWR | O_TRUNC, 0600);
// 		i ++;
// 	}
// 	unlink(file);
// 	return (fd);
// }

// void free_char(char **c)
// {
// 	free(*c);
// }


static char *get_filename()
{
	int		i;
	char	*file;
	char	*suffix;

	i = 0;
	while(true)
	{
		suffix = ft_itoa(i);
		file = ft_strjoin("mini_heredoc_", suffix);
		ezg_add(EXECUTING, suffix);
		ezg_add(EXECUTING, file);
		if (access(file, F_OK))
			break;
		i++;
	}
	return (file);
}

int	setup_heredoc(char *delimiter)
{
	/*
	 * --- Open a file
	 * --- Unlink the file
	 * Readline the input until the delimiter
	 * Return the fd
	*/
	int		fd;
	char	*input;
	char	*file;
	// fd = create_buffer();

	file = get_filename();
	// printf("FILENAME: %s\n", file);
	fd = open(file, O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, 0600);
	input = NULL;
	while (ft_strcmp(input, delimiter))
	{
		// printf("IN WHILE\n");
		input = readline("> ");
		ezg_add(EXECUTING, input);
		if (ft_strcmp(input, delimiter))
			ft_dprintf(fd, "%s\n", input);
	}
	// close(fd);
	// fd = open(file, O_RDONLY);
	unlink(file);
	return (fd);
}
