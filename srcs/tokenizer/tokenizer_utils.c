#include "../includes/minishell.h"

/**
 * Identifies token type for special characters
 */
t_token_type	identify_token_type(char *input, int *i)
{
	if (input[*i] == '|')
	{
		if (input[*i + 1] == '|')
		{
			(*i)++;
			return (TOKEN_OR);
		}
		return (TOKEN_PIPE);
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			(*i)++;
			return (TOKEN_HEREDOC);
		}
		return (TOKEN_REDIR_IN);
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			(*i)++;
			return (TOKEN_REDIR_APPEND);
		}
		return (TOKEN_REDIR_OUT);
	}
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		(*i)++;
		return (TOKEN_AND);
	}
	else if (input[*i] == '(')
		return (TOKEN_PAREN_OPEN);
	else if (input[*i] == ')')
		return (TOKEN_PAREN_CLOSE);
	return (TOKEN_WORD);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}
