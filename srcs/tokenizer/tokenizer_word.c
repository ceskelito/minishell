#include "ft_lib.h"
#include "minishell.h"

static int	count_word_length(char *word, bool *expand_dollar, bool *cat_to_next)
{
	int		len;
	int		in_quote;
	char	quote_char;

	len = 0;
	in_quote = 0;
	quote_char = 0;
	while (word[len])
	{
		if (!in_quote)
		{
			if(ft_isspace(word[len])
			|| ft_strchr("|<>&()", word[len])
			|| (len != 0 && ft_strchr("'\'", word[len])))
				break ;
		}
		if (in_quote && word[len] == quote_char)
		{
			*expand_dollar = !(quote_char == '\'');
			word++;
			in_quote = false;
			break ;
		}
/* 		if (!in_quote && (ft_isspace(word[len]) || ft_strchr("|<>&()",
					word[len])))
			break ; */
		if (!in_quote && (word[len] == '\'' || word[len] == '"'))
		{
			in_quote = 1;
			quote_char = word[len];
		}
		else if (in_quote && word[len] == quote_char)
		{
			in_quote = 0;
			quote_char = 0;
		}
		len++;
	}
	if (in_quote)
	{
		print_error("syntax error", "unclosed quote\n");
		return (-1);
	}
	//printf("word: %s\nat len: %s\n", word, word + len);
	if (word[len] && !ft_isspace(word[len]))
		*cat_to_next = true;
	return (len);
}

/* static char	*handle_word_char(char *result, char c)
{
	char	*temp;

	temp = ft_strjoin_char(result, c);
	if (!temp)
		return (NULL);
	free(result);
	return (temp);
} */

/* static char	*process_single_quote(char *input, int *i, char *result)
{
	(*i)++;
	while (input[*i] && input[*i] != '\'')
	{
		result = handle_word_char(result, input[*i]);
		if (!result)
			return (NULL);
		(*i)++;
	}
	if (input[*i] == '\'')
		(*i)++;
	return (result);
}

static char	*process_double_quote(char *input, int *i, char *result)
{
	(*i)++;
	while (input[*i] && input[*i] != '\"')
	{
		if (input[*i] == '$')
		{
			handle_dollar_sign(input, i, &result);
			if (!result)
				return (NULL);
		}
		else
		{
			result = handle_word_char(result, input[*i]);
			if (!result)
				return (NULL);
			(*i)++;
		}
	}
	if (input[*i] == '\"')
		(*i)++;
	return (result);
} */

/* static char	*expand_word_value(char *input, int len)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (input[i] == '\'')
		{
			result = process_single_quote(input, &i, result);
			if (!result)
				return (NULL);
		}
		else if (input[i] == '\"')
		{
			result = process_double_quote(input, &i, result);
			if (!result)
				return (NULL);
		}
		else if (input[i] == '$')
		{
			handle_dollar_sign(input, &i, &result);
			if (!result)
				return (NULL);
		}
		else
		{
			result = handle_word_char(result, input[i]);
			if (!result)
				return (NULL);
			i++;
		}
	}
	return (result);
} */

int	fill_word_token(t_token *token, char *input)
{
	int		len;
	//char	*expanded;

	len = count_word_length(input, &token->expand_dollar, &token->cat_to_next);
	if (len == -1)
		return (-1);
	if (len == 0)
	{
		token->value = NULL;
		token->type = WORD;
		return (0);
	}
	//expanded = expand_word_value(input, len);
	if (input[0] == '"' || input[0] == '\'')
	{
		input++;
		len--;
	}
	token->value = ft_substr(input, 0, len);
	ezg_add(GLOBAL, token->value);	
	//printf("value: %s\nexpand: %d\nconcat:%d", token->value, token->expand_dollar, token->cat_to_next);
	//exit(1);
	/* if (!expanded)
		return (-1); */
	if (!token->value)
		return (-1);
	token->type = WORD;
	return (len);
}