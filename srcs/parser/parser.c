<<<<<<< HEAD
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdollar <rdollar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-01-07 10:00:00 by rdollar           #+#    #+#             */
/*   Updated: 2025-01-07 10:00:00 by rdollar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

>>>>>>> origin/parser
#include "minishell.h"

/**
 * Creates a new command structure
 */
t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->has_pipe = 0;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

/**
 * Creates a new redirection structure
 */
t_redir	*create_redir(t_redir_type type, char *file)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

/**
 * Adds a redirection to a command
 */
void	add_redir(t_cmd *cmd, t_redir *redir)
{
	t_redir	*temp;

	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		temp = cmd->redirs;
		while (temp->next)
			temp = temp->next;
		temp->next = redir;
	}
}

/**
 * Converts token type to redirection type
 */
t_redir_type	get_redir_type(t_token_type type)
{
	if (type == REDIR_IN_TOKEN)
		return (REDIR_IN);
	else if (type == REDIR_OUT_TOKEN)
		return (REDIR_OUT);
	else if (type == REDIR_APPEND_TOKEN)
		return (REDIR_APPEND);
	else if (type == HEREDOC_TOKEN)
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

/**
 * Main parser function
 * Converts token list into command list
 */
t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_token	*current_token;

	if (!tokens)
		return (NULL);
	cmd_list = create_cmd();
	current_cmd = cmd_list;
	current_token = tokens;
	while (current_token)
	{
		if (current_token->type == PIPE_TOKEN)
		{
			current_cmd->has_pipe = 1;
			current_cmd->next = create_cmd();
			current_cmd = current_cmd->next;
		}
		else if (is_redir_token(current_token->type))
		{
			if (!parse_redirection(current_cmd, &current_token))
			{
				free_cmds(cmd_list);
				return (NULL);
			}
		}
		else if (current_token->type == WORD)
		{
			if (!add_arg(current_cmd, current_token->value))
			{
				free_cmds(cmd_list);
				return (NULL);
			}
		}
		current_token = current_token->next;
	}
	return (cmd_list);
}