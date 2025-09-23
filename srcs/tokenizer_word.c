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

char	*extract_word(char *input, int *i)
{
	char	*result;
	char	quote;

	result = ft_strdup("");
	while (input[*i] && !ft_isspace(input[*i]) &&
		!ft_strchr("|<>&()", input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"')
			result = process_quotes(input, i, result);
		else if (input[*i] == '$')
			handle_dollar_sign(input, i, &result);
		else
		{
			result = append_char(result, input[*i]);
			(*i)++;
		}
	}
	return (result);
}
