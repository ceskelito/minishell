#include "minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

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

static t_token	*create_operator_token(char *input, int *i)
{
	t_token_type	type;
	char			*value;
	t_token			*new_token;

	type = identify_token_type(input, i);
	value = create_operator_value(input, *i - (type == OR || type == AND || type == HEREDOC || type == APPEND ? 1 : 0), type);
	new_token = create_token(value, type);
	free(value);
	return (new_token);
}

t_token	*tokenize_input(char *input)
{
	t_token			*tokens;
	t_token			*new_token;
	int				i;
	char			*value;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (ft_strchr("|<>&()", input[i]))
		{
			new_token = create_operator_token(input, &i);
			i++;
		}
		else
		{
			value = extract_word(input, &i);
			if (!value)
			{
				free_tokens(tokens);
				return (NULL);
			}
			new_token = create_token(value, WORD);
			free(value);
		}
		if (new_token)
			add_token(&tokens, new_token);
	}
	return (tokens);
}