#include "ezgalloc.h"
#include "minishell.h"

/*
 * go_next_cmd - Set current command to its next
 * @curr_cmd: Pointer to current command
 * Return: 0 on success, -1 on failure
 */
static int	go_next_cmd(t_cmd **curr_cmd)
{
	(*curr_cmd)->next = ezg_calloc(COMMAND, sizeof(t_cmd), 1);
	if (!(*curr_cmd)->next)
		return (-1);
	(*curr_cmd) = (*curr_cmd)->next;
	return (0);
}

/*
 * token_count_args - Count consecutive argument tokens
 * @token: Pointer to the first token to analyze
 * Return: The number of consecutive WORD tokens
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
		argc++;
	}
	return (argc);
}

/*
 * allocate_args_array - Allocate memory for command arguments
 * @args_count: Number of arguments to allocate for
 * Return: Allocated array or NULL on failure
 */
static char	**allocate_args_array(int args_count)
{
	char	**args;

	args = ezg_calloc(COMMAND, sizeof(char *), args_count + 1);
	return (args);
}

/*
 * set_cmd_args - Initialize the argument list of a command
 * @cmd: Pointer to the command structure to populate
 * @token: Linked list of tokens representing the command arguments
 * Return: The number of arguments set on success, or -1 on failure
 */
static int	set_cmd_args(t_cmd *cmd, t_token *token)
{
	int	i;
	int	args_count;

	args_count = token_count_args(token);
	cmd->args = allocate_args_array(args_count);
	if (!cmd->args)
		return (-1);
	i = 0;
	while (i < args_count)
	{
		cmd->args[i] = ezg_add(COMMAND, ft_strdup(token->value)); // ✅ ezg_add
		if (!cmd->args[i])
			return (-1);
		token = token->next;
		i++;
	}
	cmd->args[i] = NULL;
	return (args_count);
}

/*
 * process_word_tokens - Process consecutive WORD tokens
 * @curr_cmd: Current command being built
 * @curr_token: Pointer to current token (will be advanced)
 * Return: 0 on success, -1 on failure
 */
static int	process_word_tokens(t_cmd *curr_cmd, t_token **curr_token)
{
	if (set_cmd_args(curr_cmd, *curr_token) == -1)
		return (-1);
	while ((*curr_token)->next && (*curr_token)->next->type & WORD)
		*curr_token = (*curr_token)->next;
	return (0);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_head;
	t_cmd	*curr_cmd;
	t_token	*curr_token;

	if (!tokens)
		return (NULL);
	cmd_head = ezg_calloc(COMMAND, sizeof(t_cmd), 1);
	if (!cmd_head)
		return (NULL);
	curr_cmd = cmd_head;
	curr_token = tokens;
	while (curr_token)
	{
		if (curr_token->type & WORD)
		{
			if (process_word_tokens(curr_cmd, &curr_token) == -1)
				return (NULL);
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