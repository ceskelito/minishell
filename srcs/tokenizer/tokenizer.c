#include "ezgalloc.h"
#include "minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = ezg_alloc(TOKENIZING, sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	ezg_add(TOKENIZING, token->value);
	if (!token->value)
		return (NULL);
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

//static t_token	*create_operator_token(char *input)
//{
//	t_token_type	type;
//	char			*value;
//	t_token			*new_token;
//
//	type = get_token_type(input);
//	value = get_operator_value(input, type);
//	new_token = create_token(value, type);
//	// print_tokens(new_token);
//	return (new_token);
//}

static void	fill_operator_token(t_token *token, char *input)
{
	token->type = get_token_type(input);
	token->value = get_operator_value(input, token->type);
}

static inline void	skip_spaces(char *str, int *i)
{
	while (ft_isspace(str[*i]))
			(*i)++;
}

t_token	*tokenize_input(char *input)
{
	t_token			*tokens;
	t_token			*new_token;
	int				i;
	//char			*value;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		skip_spaces(input, &i);
		continue;
		new_token = create_token(NULL, 0); // maybe change the creae token function to set all to zero
		if (ft_strchr("|<>&()", input[i]))
		{
			//new_token = create_operator_token(input + i);
			fill_operator_token(new_token, input + i);
			if (new_token->type & (OR | HEREDOC | APPEND | AND))
				i++;
			i++;
		}
		else
		{
			i += fill_word_token(new_token, input + i);
			if (!new_token->value)
				return (NULL);
			//new_token = create_token(value, WORD);
		}
		if (new_token->value)
		{
			add_token(&tokens, new_token);
			new_token = NULL;
		}
	}
	return (tokens);
}
