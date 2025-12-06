#include "ezgalloc.h"
#include "minishell.h"

static int	fill_operator_token(t_token *token, char *input)
{
	if (!token)
		return (-1);
	token->type = get_token_type(input);
	token->value = get_operator_value(input, token->type);
	return (ft_strlen(token->value));
}

t_token	*new_token()
{
	t_token *new;

	new = ezg_calloc(TOKEN, sizeof(t_token), 1);
	new->expand_dollar = true;
	new->cat_to_next = false;
	return (new);
}

t_token	*tokenize_input(char *input)
{
	t_token			*tokens;
	t_token			*new;
	t_token			*prev;
	int				token_gap;
	int				i;

	tokens = NULL;
	prev = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			(i)++;
		if (!input[i])
			break;
		new = new_token();
		if (ft_strchr("|<>", input[i]))
		{
			token_gap = fill_operator_token(new, &input[i]);
		}
		else if (prev && (prev->type & HEREDOC))
		{
			token_gap = fill_eof_token(new, &input[i]);
		}
		else
		{
			token_gap = fill_word_token(new, &input[i]);
		}
		if (token_gap == -1 || !new->value)
			return (NULL);
		add_token(&tokens, new);
		prev = new;
		new = NULL;
		i += token_gap;
	}
	return (tokens);
}
