#include "minishell.h"
#include "executor.h"
#include <stdlib.h>

static char	*append_char_safe(char *str, char c)
{
	char	*temp;

	temp = ft_strjoin_char(str, c);
	if (!temp)
		return (NULL);
	free(str);
	return (temp);
}

static char	*if_nclsd_qts(char quote_char, char *result, const char *type)
{
	if (quote_char != type[0])
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error: unclosed %s\n",
			type);
		if (result)
			free(result);
		return (NULL);
	}
	return (result);
}

static char	*handle_single_quote(char *input, int *i, char *result)
{
	(*i)++;
	while (input[*i] && input[*i] != '\'')
	{
		result = append_char_safe(result, input[*i]);
		if (!result)
			return (NULL);
		(*i)++;
	}
	return (if_nclsd_qts(input[*i], result, "'"));
}

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
			result = append_char_safe(result, input[*i]);
			if (!result)
				return (NULL);
			(*i)++;
		}
	}
	return (if_nclsd_qts(input[*i], result, "\""));
}

char	*process_quotes(char *input, int *i, char *result)
{
	char	quote;

	quote = input[*i];
	if (quote == '\'')
		return (handle_single_quote(input, i, result));
	if (quote == '\"')
		return (handle_double_quote(input, i, result));
	return (result);
}
