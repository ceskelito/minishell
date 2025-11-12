/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:49 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/12 12:55:35 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <unistd.h>

static void	add_pipe_redir(t_cmd *cmd, int fd, t_token_type type)
{
	t_redir	*redir;

	redir = ezg_calloc(GLOBAL, sizeof(t_redir), 1);
	if (!redir)
		return ;
	redir->file = NULL;
	redir->pipe_fd = fd;
	redir->type = PIPE | type;
	add_redir(cmd, redir);
}

static int 	setup_pipe(t_cmd *cmd)
{
	int	fd[2];

	if (!cmd)
		return (1);
	if (!cmd->pipe_output)
		return (0);
	if (pipe(fd) == -1)
	{
		perror("minishell");
		return (-1);
	}
	add_pipe_redir(cmd, fd[1], OUT);
	add_pipe_redir(cmd->next, fd[0], IN);
	return (0);
}

int		setup_pipeline(t_cmd *cmd_list)
{
	t_cmd	*curr;

	curr = cmd_list;
	while (curr && curr->pipe_output)
	{
		if (setup_pipe(curr) != 0)
			return (-1);
		curr = curr->next;
	}
	return (0);
}
