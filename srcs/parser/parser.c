/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:31:54 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/16 19:31:56 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezgalloc.h"
#include "minishell.h"

int	process_tokens(t_cmd **curr_cmd, t_token *curr_token);

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_head;
	t_cmd	*curr_cmd;

	if (!tokens)
		return (NULL);
	cmd_head = ezg_calloc(COMMAND, 1, sizeof(t_cmd));
	if (!cmd_head)
		return (NULL);
	curr_cmd = cmd_head;
	if (process_tokens(&curr_cmd, tokens) < 0)
		return (NULL);
	return (cmd_head);
}
