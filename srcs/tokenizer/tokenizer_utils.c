#include "minishell.h"

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*temp;

	if (!*head)
		*head = new_token;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
}

t_token_type	get_token_type(char *input)
{
	if (input[0] == '|' && input[1] == '|')
		return (OR);
	if (input[0] == '|')
		return (PIPE);
	if (input[0] == '<' && input[1] == '<')
		return (HEREDOC);
	if (input[0] == '<')
		return (IN);
	if (input[0] == '>' && input[1] == '>')
		return (APPEND);
	if (input[0] == '>')
		return (OUT);
	if (input[0] == '&' && input[1] == '&')
		return (AND);
	if (input[0] == '(')
		return (P_OPEN);
	if (input[0] == ')')
		return (P_CLOSE);
	return (WORD);
}

char	*get_operator_value(char *input, t_token_type type)
{
	if (type & (OR | AND | HEREDOC |APPEND))
		return (ezg_add(TOKEN, ft_substr(input, 0, 2)));
	else
		return (ezg_add(TOKEN, ft_substr(input, 0, 1)));
}

/* NOT USED 
int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' 
		|| c == '(' || c == ')');
} */