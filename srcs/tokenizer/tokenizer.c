#include "ezgalloc.h"
#include "minishell.h"

/*
 * create_token - Creates a new token with given value and type
 */
t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new;

	new = ezg_calloc(TOKEN, sizeof(t_token), 1);
	if (!new)
		return (NULL);
	new->value = ezg_add(TOKEN, ft_strdup(value));
	new->type = type;
	new->expand_dollar = true;
	new->next = NULL;
	return (new);
}

static int	fill_operator_token(t_token *token, char *input)
{
	if (!token)
		return (-1);
	token->type = get_token_type(input);
	token->value = get_operator_value(input, token->type);
	return (ft_strlen(token->value));
}

static t_token	*new_token(void)
{
	t_token *new;

	new = ezg_calloc(TOKEN, sizeof(t_token), 1);
	if (!new)
		return (NULL);
	new->expand_dollar = true;
	return (new);
}

t_token	*tokenize_input(char *input)
{
	t_token			*tokens;
	t_token			*new;
	int				token_gap;
	int				i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break;
			
		if (ft_strchr("|<>&()", input[i]))
		{
			new = new_token();
			if (!new)
				return (NULL);
			token_gap = fill_operator_token(new, input + i);
			if (token_gap == -1 || !new->value)
				return (NULL);
			add_token(&tokens, new);
			i += token_gap;
		}
		else
		{
			new = new_token();
			if (!new)
				return (NULL);
			token_gap = fill_word_token(new, input + i);
			if (token_gap == -1 || !new->value)
				return (NULL);
			add_token(&tokens, new);
			i += token_gap;
		}
	}
	return (tokens);
}