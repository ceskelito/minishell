#include "minishell.h"

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = ezg_alloc(GLOBAL, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->pipe_output = 0;
	return (cmd);
}

t_redir	*create_redir(int type, char *file)
{
	t_redir	*redir;

	redir = ezg_alloc(GLOBAL, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ezg_add(GLOBAL, ft_strdup(file));
	if (!redir->file)
		return (NULL);
	redir->next = NULL;
	return (redir);
}

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

static t_cmd	*handle_pipe_token(t_cmd *current_cmd)
{
	t_cmd	*new_cmd;

	current_cmd->pipe_output = 1;
	new_cmd = create_cmd();
	if (!new_cmd)
		return (NULL);
	current_cmd->next = new_cmd;
	return (new_cmd);
}

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
			current_cmd = handle_pipe_token(current_cmd);
			if (!current_cmd)
			{
				free_cmds(cmd_list);
				return (NULL);
			}
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