#include "ft_lib.h"
#include "minishell.h"

/* static char	*append_char_safe(char *str, char c)
{
	char	*temp;

	temp = ft_strjoin_char(str, c);
	if (!temp)
		return (NULL);
	free(str);
	return (temp);
} */

static char	*handle_double_quote(char *input, int *i, char *result)
{
	char	*temp;

	(*i)++;
	while (input[*i] && input[*i] != '\"')
	{
		if (input[*i] == '$')
		{
			temp = result;
			handle_dollar_sign(input, i, &result);
			if (!result)
			{
				if (temp)
					free(temp);
				return (NULL);
			}
		}
		else
		{
			result = ft_strjoin_char(result, input[*i]); //append_char_safe(result, input[*i]);
			if (!result)
				return (NULL);
			(*i)++;
		}
	}
	if (input[*i] != '\"')
	{
		printf("minishell: syntax error: unclosed double quote\n");
		if (result)
			free(result);
		return (NULL);
	}
	(*i)++;
	return (result);
}

static char	*handle_single_quote(char *input, int *i, char *result)
{
	(*i)++;
	while (input[*i] && input[*i] != '\'')
	{
		result = ft_strjoin_char(result, input[*i]); //append_char_safe(result, input[*i]);
		if (!result)
			return (NULL);
		(*i)++;
	}
	if (input[*i] != '\'')
	{
		printf("minishell: syntax error: unclosed single quote\n");
		if (result)
			free(result);
		return (NULL);
	}
	(*i)++;
	return (result);
}

char	*process_quotes(char *input, int *i, char *result)
{
	char	quote;

	quote = input[*i];
	if (quote == '\'')
		return (handle_single_quote(input, i, result));
	else if (quote == '\"')
		return (handle_double_quote(input, i, result));
	return (result);
}

static int	__handle_single_quote(char *dest, char *input, int start)
{
	int	len;
	int	i;

	i = start + 1;
	len = 0;
	while (input[i] && input[i] != '\'')
	{
		i++;
		len++;
	}
	if (input[i] != '\'')
	{
		printf("minishell: syntax error: unclosed single quote\n");
		return (-1);
	}
	dest = ft_substr(input, start + 1, len);
	if (!dest)
		return (-1);
	return (i);
}

char *refine_word(char *raw_word)
{
	char	*refined;
	int		gap;
	int		i;

	refined = NULL;
	i = 0;
	while(raw_word[i])
	{
		if (raw_word[i] == '\'')
			gap = __handle_single_quote(refined, raw_word, i);
		if (gap == -1)
			return (NULL);
		i += gap;
	}
	if (refined)
		return (refined);
	return (raw_word);
}