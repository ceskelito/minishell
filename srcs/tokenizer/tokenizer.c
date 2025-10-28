#include "ezgalloc.h"
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

int	is_special_in_word(char c)
{
	return (c == '$' || c == '\'' || c == '\"');
}

static void	fill_operator_token(t_token *token, char *input)
{
	if (!token)
		return ;
	token->type = get_token_type(input);
	token->value = get_operator_value(input, token->type);
}

static t_token	*new_token()
{
	t_token *new;

	new = ezg_calloc(TOKENIZING, sizeof(t_token), 1);
	new->expand_dollar = true;
	return (new);
}

t_token	*tokenize_input(char *input)
{
	t_token			*tokens;
	t_token			*new;
	int				i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			(i)++;
		if (!input[i])
			break;
		new = new_token();
		if (ft_strchr("|<>&()", input[i]))
		{
			fill_operator_token(new, input + i);
			if (new->type & (OR | HEREDOC | APPEND | AND))
				i++;
			i++;
		}
		else
		{
			i += fill_word_token(new, input + i);
			if (!new->value)
				return (NULL);
		}
		if (new->value)
		{
			add_token(&tokens, new);
			new = NULL;
		}
	}
	return (tokens);
}
