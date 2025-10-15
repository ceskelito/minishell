#include "minishell.h"

static int	count_word_length(char *input, int start)
{
	int	len;
	int	in_quote;
	char quote_char;

	len = 0;
	in_quote = 0;
	quote_char = 0;
	while (input[start + len])
	{
		if (!in_quote && ft_isspace(input[start + len]))
			break;
		if (!in_quote && ft_strchr("|<>&()", input[start + len]))
			break;
		if (!in_quote && (input[start + len] == '\'' || input[start + len] == '\"'))
		{
			in_quote = 1;
			quote_char = input[start + len];
		}
		else if (in_quote && input[start + len] == quote_char)
		{
			in_quote = 0;
			quote_char = 0;
		}
		len++;
	}
	if (in_quote)
		return (-1); // unclosed quote
	return (len);
}

static char	*copy_word_simple(char *input, int start, int len)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (input[start + i] != '\'' && input[start + i] != '\"')
		{
			result[j] = input[start + i];
			j++;
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*extract_word(char *input, int *i)
{
	int		len;
	char	*result;
	int		start;

	start = *i;
	len = count_word_length(input, start);
	if (len == -1)
	{
		printf("minishell: syntax error: unclosed quote\n");
		return (NULL);
	}
	if (len == 0)
		return (ft_strdup(""));
	result = copy_word_simple(input, start, len);
	*i += len;
	return (result);
}