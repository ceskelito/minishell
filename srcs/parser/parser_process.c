/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_process.c                                   :+:      :+:    :+:   */
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

static bool	cmd_has_content(t_cmd *cmd)
{
	return (cmd->args != NULL || cmd->redirs != NULL);
}

static int	handle_word(t_cmd **curr_cmd, t_token **curr_token)
{
	if (handle_word_token(*curr_cmd, curr_token) < 0)
		return (-1);
	return (0);
}

static int	handle_redir(t_cmd **curr_cmd, t_token **curr_token)
{
	if (handle_redir_token(*curr_cmd, curr_token) < 0)
		return (-1);
	return (0);
}

static int	handle_pipe(t_cmd **curr_cmd, t_token *curr_token)
{
	if (!cmd_has_content(*curr_cmd))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		return (-1);
	}
	if (handle_pipe_token(curr_cmd, curr_token) < 0)
		return (-1);
	return (0);
}

int	process_tokens(t_cmd **curr_cmd, t_token *curr_token)
{
	while (curr_token)
	{
		if (curr_token->type & WORD)
		{
			if (handle_word(curr_cmd, &curr_token) < 0)
				return (-1);
		}
		else if (is_redir_token(curr_token->type))
		{
			if (handle_redir(curr_cmd, &curr_token) < 0)
				return (-1);
		}
		else if (curr_token->type & PIPE)
		{
			if (handle_pipe(curr_cmd, curr_token) < 0)
				return (-1);
		}
		curr_token = curr_token->next;
	}
	return (0);
}
