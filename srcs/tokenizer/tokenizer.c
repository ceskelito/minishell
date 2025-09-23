#include "../includes/minishell.h"

/**
 * Creates a new token structure
 */
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

/**
 * Adds a token to the end of the token list
 */
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

/**
 * Main tokenization function
 * Converts input string into a linked list of tokens
 */
t_token	*tokenize_input(char *input)
{
	t_token			*tokens;
	t_token			*new_token;
	t_token_type	type;
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
			type = identify_token_type(input, &i);
			value = ft_strdup("");
			i++;
		}
		else
		{
			type = TOKEN_WORD;
			value = extract_word(input, &i);
		}
		new_token = create_token(value, type);
		free(value);
		add_token(&tokens, new_token);
	}
	return (tokens);
}
