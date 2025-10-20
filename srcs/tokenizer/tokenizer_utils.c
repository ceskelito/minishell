#include "minishell.h"

t_token_type	get_token_type(char *input)
{
	if (*input == '|')
	{
		if (&input[1] && input[1] == '|')
			return (OR);
		return (PIPE);
	}
	if (*input == '<')
	{
		if (&input[1] && input[1] == '<')
			return (HEREDOC);
		return (IN);
	}
	if (*input == '>')
	{
		if (&input[1] && input[1] == '>')
			return (APPEND);
		return (OUT);
	}
	if (*input == '&' && input[1] == '&')
		return (AND);
	if (*input == '(')
		return (P_OPEN);
	if (*input == ')')
		return (P_CLOSE);
	return (WORD);
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' 
		|| c == '(' || c == ')');
}

char	*get_operator_value(char *input, t_token_type type)
{
	if (type & (OR | AND | HEREDOC |APPEND))
		return (ft_substr(input, 0, 2));
	else
		return (ft_substr(input, 0, 1));
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