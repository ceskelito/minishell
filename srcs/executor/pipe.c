/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:49 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 16:26:36 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipe(t_redir *first, t_redir *second)
{
	int	fd[2];

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

int	open_pipeline_fds(t_cmd *cmd_list)
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
