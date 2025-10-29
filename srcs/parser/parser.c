#include "ezgalloc.h"
#include "minishell.h"

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = ezg_calloc(COMMAND, sizeof(t_cmd), 1);
	if (!cmd)
		return (NULL);
	return (cmd);
}

t_redir	*create_redir(int type, char *file)
{
	t_redir	*redir;

	redir = ezg_alloc(COMMAND, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ezg_add(COMMAND, ft_strdup(file));
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

/* Set @curr_cmd to his next. */
static int	go_next_cmd(t_cmd **curr_cmd)
{
	(*curr_cmd)->next = create_cmd();
	if (!(*curr_cmd)->next)
		return (-1);
	(*curr_cmd) = (*curr_cmd)->next;
	return (0);
}

/*
 * token_count_args - Count consecutive argument tokens.
 *
 * @token: Pointer to the first token to analyze.
 *
 * Count consecutive WORD tokens starting from the given token.
 * Used to determine the number of arguments for the next command.
 *
 * Return: The number of consecutive WORD tokens.
 */
static int	token_count_args(t_token *token)
{
	t_token *tmp;
	int		argc;
	
	argc = 0;
	tmp = token;
	while (tmp && tmp->type & WORD)
	{
		tmp = tmp->next;
		argc ++;
	}
	return (argc);
}

/*
 set_cmd_args - Initialize the argument list of a command.

 @cmd:   Pointer to the command structure to populate.
 @token: Linked list of tokens representing the command arguments.

 Allocate and fill the cmd->arg array duplicating each token value
 of the provided token list.
 The caller is responsible for advancing or
 skipping the used tokens outside of this function.

 Return: The number of arguments set on success, or -1 on allocation failure.
 */
static int set_cmd_args(t_cmd *cmd, t_token *token)
{
	int	i;
	int	args_count;

	args_count = token_count_args(token);
	cmd->args = ezg_calloc(COMMAND, sizeof(char *), args_count + 1);
	if (!cmd->args)
		return (-1);
	i = 0;
	while (i < args_count)
	{
		cmd->args[i] = ft_strdup(token->value);
		if (!cmd->args[i])
			return (-1);
		ezg_add(COMMAND, cmd->args[i]);
		token = token->next;
		i++;
	}
	cmd->args[i] = NULL;
	return (args_count);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_head;
	t_cmd	*curr_cmd;
	t_token	*curr_token;

	if (!tokens)
		return (NULL);
	cmd_head = create_cmd();
	if (!cmd_head)
		return (NULL);
	curr_cmd = cmd_head;
	curr_token = tokens;
	while (curr_token)
	{
		if (curr_token->type & WORD)
		{
			if (set_cmd_args(curr_cmd, curr_token) == -1)
				return (NULL);
			while (curr_token->next && curr_token->next->type & WORD)
				curr_token = curr_token->next;
		}
		else if (is_redir_token(curr_token->type))
		{
			if (parse_redirection(curr_cmd, &curr_token) != 0)
				return (NULL);
		}
		else if (curr_token->type & PIPE)
		{
			curr_cmd->pipe_output = true;
			if (go_next_cmd(&curr_cmd) != 0)
				return (NULL);
		}
		curr_token = curr_token->next;
	}
	return (cmd_head);
}