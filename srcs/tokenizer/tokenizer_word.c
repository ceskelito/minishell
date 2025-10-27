#include "ft_lib.h"
#include "minishell.h"

static int	count_word_length(char *input)
{
	int	len;
	int	in_quote;
	char quote_char;

	len = 0;
	in_quote = 0;
	quote_char = 0;
	while (input[len])
	{
		if (!in_quote && (ft_isspace(input[len]) || ft_strchr("|<>&()", input[len])))
			break;
		if (!in_quote && (input[len] == '\'' || input[len] == '"'))
		{		
			in_quote = 1;
			quote_char = input[len];
		}
		else if (in_quote && input[len] == quote_char)
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

static char	*copy_word_simple(char *input, int word_len)
{
	char	*word;
	int		i;
	int		j;

	word = ezg_alloc(GLOBAL, sizeof(char) * (word_len + 1));
	if (!word)
		return (NULL);
	i = 0;
	j = 0;
	while (i < word_len)
	{
		if (input[i] != '\'' && input[i] != '\"')
		{
			word[j] = input[i];
			j++;
			word[j] = '\0';
		}
		else
			word = process_quotes(input, &i, word); // don't work
		i++;
	}
	printf("EXT result: %s\n", word); //DEBUG
	return (word);
}

/* char	*refine_word(char *raw, int word_len)
{
	char	*word;
	int		i;

	i = 0;
	while(word[i])
	{

		i++;
	}
} */

int		fill_word_token(t_token *token, char *input)
{
	int		word_len;
	char	*raw_word;

	word_len = count_word_length(input);
	if (word_len == -1)
		return (-1);
	if (word_len == 0)
		token->value = ft_strdup("");
	else
	{
		raw_word = ft_substr(input, 0, word_len);
		printf("WORD: %s\n", raw_word);
		token->value = refine_word(raw_word);
		//token->value = copy_word_simple(input, word_len);
	}
	token->type = WORD;
	//ezg_add(TOKENIZING, token->value);
	return (word_len);
}