#include "ezgalloc.h"
#include "minishell.h"

/*
 * create_token - Creates a new token with given value and type
 * @value: String value for the token
 * @type: Token type (WORD, PIPE, etc.)
 * Return: New token or NULL on failure
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

/*
 * process_attached_quotes - Handles attached quotes as separate tokens
 * Example: "$ONE""two" becomes two tokens: "$ONE" and "two"
 * @input: Input string starting at current position
 * @i: Pointer to current position in input
 * @tokens: Token list to add new tokens to
 * Return: Number of characters processed or -1 on error
 */
static int	process_attached_quotes(char *input, int *i, t_token **tokens)
{
	int		start;
	int		len;
	char	*token_value;
	t_token	*new;

	start = *i;
	len = 0;
	
	// Process each quoted section as separate token
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&()", input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			// If we have accumulated characters, create token for them
			if (len > 0)
			{
				token_value = ezg_add(TOKEN, ft_substr(input, start, len));
				new = create_token(token_value, WORD);
				if (!new)
					return (-1);
				add_token(tokens, new);
			}
			
			// Process the quoted part as separate token
			start = *i;
			char quote = input[*i];
			(*i)++; // Skip opening quote
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (input[*i] == quote)
				(*i)++; // Skip closing quote
			
			len = *i - start;
			token_value = ezg_add(TOKEN, ft_substr(input, start, len));
			new = create_token(token_value, WORD);
			if (!new)
				return (-1);
			if (quote == '\'')
				new->expand_dollar = false;
			add_token(tokens, new);
			
			// Reset for next part
			start = *i;
			len = 0;
		}
		else
		{
			(*i)++;
			len++;
		}
	}
	
	// Handle remaining characters
	if (len > 0)
	{
		token_value = ezg_add(TOKEN, ft_substr(input, start, len));
		new = create_token(token_value, WORD);
		if (!new)
			return (-1);
		add_token(tokens, new);
	}
	
	return (*i - start + len);
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
			// Check if this word has attached quotes
			int temp_i = i;
			bool has_quotes = false;
			while (input[temp_i] && !ft_isspace(input[temp_i]) && !ft_strchr("|<>&()", input[temp_i]))
			{
				if (input[temp_i] == '\'' || input[temp_i] == '\"')
				{
					has_quotes = true;
					break;
				}
				temp_i++;
			}
			
			if (has_quotes)
			{
				token_gap = process_attached_quotes(input, &i, &tokens);
				if (token_gap == -1)
					return (NULL);
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
	}
	return (tokens);
}
