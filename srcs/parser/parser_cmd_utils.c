/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:32:28 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/16 19:32:33 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ezgalloc.h"

int	go_next_cmd(t_cmd **cmd)
{
	(*cmd)->next = ezg_calloc(COMMAND, 1, sizeof(t_cmd));
	if (!(*cmd)->next)
		return (-1);
	*cmd = (*cmd)->next;
	return (0);
}

void	add_pipe_redir(t_cmd *cmd, t_token_type type)
{
	t_redir	*r;

	r = ezg_calloc(GLOBAL, 1, sizeof(t_redir));
	if (!r)
		return ;
	r->type = PIPE | type;
	r->pipe_fd = -1;
	r->file = NULL;
	add_redir(cmd, r);
}
