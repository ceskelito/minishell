#include "minishell.h"

enum {NO_QUOTES = -2, UNCLOSED_QUOTES = -1};
enum {IN_QUOTES = true, OUT_QUOTES = false};

static inline bool	isquote(char c)
{
	return (c == '\'' || c == '"');	
}

static inline bool isspecial(char c)
{
	return (ft_strchr("|<>", c));
}

static void	set_token_value(t_token *token, char *input, int gap, int len, bool in_quote)
{
	char	*result;

	if (in_quote)
		token->expand_dollar = false;	
	if (input[gap + 1] && !ft_isspace(input[gap + 1]))
		token->cat_to_next = true;
	if (len == 0)
		result = ft_strdup("");
	else
		result = ft_substr(input, in_quote, len);
	if (!result)
	{
		perror("minishell:");
		exit(errno);
	}
	ezg_add(TOKEN, result);
	token->value = result;
	
}

static int	process_word_surrounded(t_token *token, char *input)
{
	int		gap;
	int		len;
	char	quote;
	char	*result;

	gap = 1;
	len = 0;
	quote = input[0];
	while (input[gap] && input[gap] != quote)
	{
		len++;
		gap++;
	}
	gap++;
	if (input[gap] != quote)
	{
		ft_dprintf(STDERR_FILENO, "minishell: unexpected EOF while looking for matching `%c", quote);
		print_error("syntax error", "unexpected end of file");
		return (UNCLOSED_QUOTES);
	}
	set_token_value(token, input, gap, len, IN_QUOTES);
	return (gap);
}

int	process_word_nosurround(t_token *token, char *input)
{
	int		gap;
	int		len;
	char	*result;

	gap = 0;
	len = 0;
	while (input[gap] && !( isquote(input[gap]) || ft_isspace(input[gap]) || isspecial(input[gap]) ))
	{
		len++;
		gap++;
	}
	set_token_value(token, input, gap, len, OUT_QUOTES);
	return (gap);
}

int	fill_word_token(t_token *token, char *input)
{
	int		len;
	int		gap;
	int		spaces;
	char	*value;

	spaces = 0;
	while (ft_isspace(*input))
	{	
		spaces++;
		input++;
	}
	gap = 0;
	if (isquote(input[0]))
		gap = process_word_surrounded(token, input);
	else
		gap = process_word_nosurround(token, input);
	if (gap < 0)
		return (gap);
	token->type = WORD;
	return (gap + spaces);
}
