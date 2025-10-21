#include "minishell.h"

static int	count_word_length(char *word)
{
	int	len;
	int	in_quote;
	char quote_char;

	len = 0;
	in_quote = 0;
	quote_char = 0;
	while (word[len])
	{
		if (!in_quote && (ft_isspace(word[len]) || ft_strchr("|<>&()", word[len])))
			break;
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
		return (-1); // unclosed quote
	return (len);
}

static char	*copy_word_simple(char *input, int start, int len)
{
	char	*result;
	int		i;
	int		j;

	result = ezg_alloc(PARSING, sizeof(char) * (len + 1));
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

//static char *new_copy_word(char *word, int len)
//{
//	char	*result;
//
//	result = ezg_alloc(PARSING, sizeof(char) * len);
//	if (word[0] == '"' || word[0] == '\'')
//		ft_strlcpy(result, word + 1, len - 1);
//	else
//		ft_strlcpy(result, word, len + 1);
//	return (result);
//}

char	*extract_word(char *input, int *i)
{
	int		len;
	char	*result;
	int		start;

	start = *i;
	len = count_word_length(input + start);
	if (len == -1)
	{
		perror("minishell: syntax error: unclosed quote\n");
		return (NULL);
	}
	if (len == 0)
		return (ft_strdup(""));
	result = copy_word_simple(input, start, len);
	//result = new_copy_word(input + start, len);
	*i += len;
	return (result);
}

// A new word occur when, out of the quotes, there's a space
