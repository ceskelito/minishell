#include "minishell.h"

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
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			(*i)++;
			return (HEREDOC);
		}
		return (IN);
	}
	if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			(*i)++;
			return (APPEND);
		}
		return (OUT);
	}
	if (input[*i] == '&' && input[*i + 1] == '&')
	{
		(*i)++;
		return (AND);
	}
	if (input[*i] == '(')
		return (P_OPEN);
	if (input[*i] == ')')
		return (P_CLOSE);
	return (WORD);
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' 
		|| c == '(' || c == ')');
}

char	*create_operator_value(char *input, int pos, t_token_type type)
{
	if (type == OR || type == AND || type == HEREDOC || type == APPEND)
		return (ft_substr(input, pos, 2));
	else
		return (ft_substr(input, pos, 1));
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