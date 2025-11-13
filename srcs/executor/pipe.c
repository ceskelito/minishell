/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:49 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/13 17:29:46 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <unistd.h>

/* static void	add_pipe_redir(t_cmd *cmd, int fd, t_token_type type)
{
	t_redir	*redir;

	redir = ezg_calloc(GLOBAL, sizeof(t_redir), 1);
	if (!redir)
		return ;
	redir->file = NULL;
	redir->pipe_fd = fd;
	redir->type = PIPE | type;
	add_redir(cmd, redir);
} */

static int 	setup_pipe(t_redir *first, t_redir *second)
{
	int		fd[2];

	if (pipe(fd) == -1)
	{
		perror("minishell");
		return (-1);
	}
	while (first && first->type != (PIPE | OUT))
		first = first->next;
	if (first)
		first->pipe_fd = fd[1];
	while (second && second->type != (PIPE | IN))
		second = second->next;
	if (second)
		second->pipe_fd = fd[0];
	return (0);
}

int		setup_pipeline(t_cmd *cmd_list)
{
	t_cmd	*curr;

	curr = cmd_list;
	while (curr && curr->pipe_output)
	{
		if (setup_pipe(curr->redirs, curr->next->redirs) != 0)
			return (-1);
		curr = curr->next;
	}
	return (0);
}
