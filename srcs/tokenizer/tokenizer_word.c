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
	{
		perror("minishell: syntax error: unclosed quote\n");
		return (-1);
	}
	return (len);
}

static char	*copy_word_simple(t_token *token, char *input, int len)
{
	int		i;
	int		j;

	token->value = ezg_alloc(GLOBAL, sizeof(char) * (len + 1));
	if (!token->value)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (input[i] == '\'')
			token->expand_dollar = false;
		if (input[i] != '\'' && input[i] != '\"')
		{
			token->value[j] = input[i];
			j++;
		}
		i++;
	}
	token->value[j] = '\0';
	return (token->value);
}

//static char *new_copy_word(char *word, int len)
//{
//	char	*result;
//
//	result = ezg_alloc(COMMAND, sizeof(char) * len);
//	if (word[0] == '"' || word[0] == '\'')
//		ft_strlcpy(result, word + 1, len - 1);
//	else
//		ft_strlcpy(result, word, len + 1);
//	return (result);
//}

//int	set_word_value(char *result, char *input)
//{
//	int		len;
//
//	result = NULL;
//	len = count_word_length(input);
//	if (len == -1)
//		return (-1);
//	if (len == 0)
//		result = ft_strdup("");
//	else
//		result = copy_word_simple(input, len);
//	ezg_add(TOKEN, result);
//	return (len);
//}

int	fill_word_token(t_token *token, char *input)
{
	int		len;

	len = count_word_length(input);
	if (len == -1)
		return (-1);
	if (len == 0)
		token->value = NULL; //ft_strdup("");
	else
		copy_word_simple(token, input, len);
	token->type = WORD;
	//ezg_add(TOKEN, token->value);
	return (len);
}