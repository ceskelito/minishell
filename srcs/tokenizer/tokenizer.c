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

/*
 * find_quote_end - Find the end of a quoted section
 */
static int	find_quote_end(char *input, int start, char quote_char)
{
	int	i;

	i = start + 1;
	while (input[i])
	{
		if (input[i] == quote_char)
			return (i + 1);
		i++;
	}
	return (-1);
}

/*
 * extract_quote - Extract content between quotes (including quotes)
 */
static char	*extract_quote(char *input, int start, int end)
{
	return (ezg_add(TOKEN, ft_substr(input, start, end - start)));
}

/*
 * process_quotes - Handle word that may contain attached quotes
 * Example: "$ONE""two"three -> ["$ONE"] ["two"] [three]
 */
static int	process_quotes(char *input, int *i, t_token **tokens)
{
	int		start_pos;
	int		current_pos;
	char	*token_value;
	t_token	*new;

	start_pos = *i;
	current_pos = *i;

	while (input[current_pos] && !ft_isspace(input[current_pos]) 
		   && !ft_strchr("|<>&()", input[current_pos]))
	{
		if (input[current_pos] == '\'' || input[current_pos] == '\"')
		{
			if (current_pos > start_pos)
			{
				token_value = ezg_add(TOKEN, ft_substr(input, start_pos, current_pos - start_pos));
				new = create_token(token_value, WORD);
				if (!new)
					return (-1);
				add_token(tokens, new);
			}

			char quote_char = input[current_pos];
			int quote_end = find_quote_end(input, current_pos, quote_char);
			
			if (quote_end == -1)
			{
				ft_dprintf(STDERR_FILENO, "minishell: syntax error: unclosed quote\n");
				return (-1);
			}

			token_value = extract_quote(input, current_pos, quote_end);
			new = create_token(token_value, WORD);
			if (!new)
				return (-1);
			
			if (quote_char == '\'')
				new->expand_dollar = false;
				
			add_token(tokens, new);

			current_pos = quote_end;
			start_pos = current_pos;
		}
		else
		{
			current_pos++;
		}
	}

	if (current_pos > start_pos)
	{
		token_value = ezg_add(TOKEN, ft_substr(input, start_pos, current_pos - start_pos));
		new = create_token(token_value, WORD);
		if (!new)
			return (-1);
		add_token(tokens, new);
	}

	*i = current_pos;
	return (current_pos - start_pos);
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
			token_gap = process_quotes(input, &i, &tokens);
			if (token_gap == -1)
				return (NULL);
		}
	}
	return (tokens);
}