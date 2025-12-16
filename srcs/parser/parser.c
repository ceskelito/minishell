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

int		handle_word_token(t_cmd *curr_cmd, t_token **curr_token);
int		handle_redir_token(t_cmd *curr_cmd, t_token **curr_token);
int		handle_pipe_token(t_cmd **curr_cmd, t_token *curr_token);
int		go_next_cmd(t_cmd **curr_cmd);

static bool	cmd_has_content(t_cmd *cmd)
{
	return (cmd->args != NULL || cmd->redirs != NULL);
}

static int	process_tokens(t_cmd **curr_cmd, t_token *curr_token)
{
	while (curr_token)
	{
		if (curr_token->type & WORD)
		{
			if (handle_word_token(*curr_cmd, &curr_token) < 0)
				return (-1);
		}
		else if (is_redir_token(curr_token->type))
		{
			if (handle_redir_token(*curr_cmd, &curr_token) < 0)
				return (-1);
		}
		else if (curr_token->type & PIPE)
		{
			if (!cmd_has_content(*curr_cmd))
			{
				ft_dprintf(STDERR_FILENO,
					"minishell: syntax error near unexpected token `|'\n");
				return (-1);
			}
			if (handle_pipe_token(curr_cmd, curr_token) < 0)
				return (-1);
		}
		curr_token = curr_token->next;
	}
	return (0);
}

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
