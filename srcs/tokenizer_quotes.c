#include "minishell.h"

static char	*append_char(char *str, char c)
{
	char	*temp;
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	temp = ft_strjoin(str, buf);
	free(str);
	return (temp);
}

char	*process_quotes(char *input, int *i, char *result)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
	{
		if (quote == '\"' && input[*i] == '$')
			handle_dollar_sign(input, i, &result);
		else
		{
			result = append_char(result, input[*i]);
			(*i)++;
		}
	}
	if (input[*i] == quote)
		(*i)++;
	return (result);
}
