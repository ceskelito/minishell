/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:47 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 16:26:31 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Need to expand variables
// Need to manage cases with EOF in quotes

static bool	process_eof_quotes(char **delimiter)
{
	char	*str;

	str = *delimiter;
	if (!str)
		return (false);
	if (ft_strcmp(str, "") == 0)
		return (true);
	if (str[0] == '\'' || str[0] == '"')
	{
		str = ft_substr(*delimiter, 1, ft_strlen(*delimiter) - 2);
		ezg_add(EXECUTING, str);
		*delimiter = str;
		return (true);
	}
	return (false);
}

static bool	get_heredoc_line(char *delimiter, int fd)
{
	char	*input;
	bool	expand;

	expand = !process_eof_quotes(&delimiter);
	input = readline("> ");
	if (!input)
	{
		print_error("warning", "heredoc terminated by EOF\n");
		return (false);
	}
	ezg_add(EXECUTING, input);
	if (ft_strcmp(input, delimiter) == 0)
		return (false);
	if (expand)
		input = ezg_add(EXECUTING, string_expand_dollars(input));
	if (input == NULL)
	{
		perror("minishell");
		set_exit_status(errno);
		exit_shell(NULL);
	}
	ft_dprintf(fd, "%s\n", input);
	return (true);
}

static int	process_heredoc(char *delimiter)
{
	int	fd[2];

	if (pipe(fd) == -1)
	{
		perror("minishell");
		return (-1);
	}
	while (get_heredoc_line(delimiter, fd[1]))
		;
	close(fd[1]);
	return (fd[0]);
}

void	setup_heredocs(t_cmd *cmd_list)
{
	t_cmd	*curr;
	t_redir	*curr_redir;

	curr = cmd_list;
	while (curr)
	{
		curr_redir = curr->redirs;
		while (curr_redir)
		{
			if (curr_redir->type & HEREDOC)
			{
				curr_redir->pipe_fd = process_heredoc(curr_redir->file);
			}
			curr_redir = curr_redir->next;
		}
		curr = curr->next;
	}
}
