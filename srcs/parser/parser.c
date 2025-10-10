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
	cmd->pipe_output = 0;
	return (cmd);
}

/**
 * Creates a new redirection structure
 * type uses your enum values: IN, OUT, APPEND, HEREDOC
 */
t_redir	*create_redir(int type, char *file)
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
 * Main parser function
 * Converts token list into command list
 */
t_cmd	*parse_tokens(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_token	*current_token;

	(void)shell;
	if (!tokens)
		return (NULL);
	cmd_list = create_cmd();
	if (!cmd_list)
		return (NULL);
	current_cmd = cmd_list;
	current_token = tokens;
	while (current_token)
	{
		if (current_token->type & PIPE)
		{
			current_cmd->pipe_output = 1;
			current_cmd->next = create_cmd();
			if (!current_cmd->next)
			{
				free_cmds(cmd_list);
				return (NULL);
			}
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