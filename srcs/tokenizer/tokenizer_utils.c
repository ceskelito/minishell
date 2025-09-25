#include "minishell.h"

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
			return (OR);
		}
		return (PIPE);
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			(*i)++;
			return (HEREDOC);
		}
		return (IN);
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			(*i)++;
			return (APPEND);
		}
		return (OUT);
	}
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		(*i)++;
		return (AND);
	}
	else if (input[*i] == '(')
		return (P_OPEN);
	else if (input[*i] == ')')
		return (P_CLOSE);
	return (WORD);
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
