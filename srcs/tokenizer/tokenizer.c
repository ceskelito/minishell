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

static int	fill_operator_token(t_token *token, char *input)
{
	if (!token)
		return (0);
	token->type = get_token_type(input);
	token->value = get_operator_value(input, token->type);
	return (ft_strlen(token->value));
}

t_token	*tokenize_input(char *input)
{
	t_token			*tokens;
	t_token			*new_token;
	int				token_gap; // The lenght occupied from the token in the input string
	int				i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		token_gap = 0;
		while (ft_isspace(input[i]))
			(i)++;
		if (!input[i])
			break;
		new_token = ezg_calloc(TOKENIZING, sizeof(t_token), 1);
		if (ft_strchr("|<>&()", input[i]))
		{
			token_gap = fill_operator_token(new_token, input + i);
		}
		else
		{
			token_gap = fill_word_token(new_token, input + i);
		}
		if (token_gap == -1 || !new_token->value)
		{	
			perror("minishell: failed in token creation");
			return (NULL);
		}
		i += token_gap;
		add_token(&tokens, new_token);
		new_token = NULL;
	}
	return (tokens);
}
