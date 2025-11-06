/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:47 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/06 12:42:47 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		perror("minishell");
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
