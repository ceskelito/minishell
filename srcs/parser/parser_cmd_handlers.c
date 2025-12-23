/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:32:28 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/16 19:32:33 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ezgalloc.h"

// Skip all WORD tokens

int	handle_word_token(t_cmd *cmd, t_token **token)
{
	if (set_cmd_args(cmd, *token) < 0)
		return (-1);
	while ((*token)->next && ((*token)->next->type & WORD))
		*token = (*token)->next;
	return (0);
}

int	handle_redir_token(t_cmd *cmd, t_token **token)
{
	if (parse_redirection(cmd, token) != 0)
		return (-1);
	return (0);
}

// Check if there is a file redirect to stdout
static bool	has_stdout_redir(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if ((redir->type & OUT) && !(redir->type & PIPE))
			return (true);
		if ((redir->type & APPEND) && !(redir->type & PIPE))
			return (true);
		redir = redir->next;
	}
	return (false);
}

static bool	has_stdin_redir(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if ((redir->type & IN) && !(redir->type & PIPE))
			return (true);
		if ((redir->type & HEREDOC) && !(redir->type & PIPE))
			return (true);
		redir = redir->next;
	}
	return (false);
}

int	handle_pipe_token(t_cmd **cmd, t_token *token)
{
	if (!token->next)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (token->next->type & PIPE)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		return (-1);
	}
	(*cmd)->pipe_output = true;
	if (!has_stdout_redir(*cmd))
		add_pipe_redir(*cmd, OUT);
	if (go_next_cmd(cmd) < 0)
		return (-1);
	if (!has_stdin_redir(*cmd))
		add_pipe_redir(*cmd, IN);
	return (0);
}
