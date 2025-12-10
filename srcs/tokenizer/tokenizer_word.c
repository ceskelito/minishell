/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:07:35 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/06 20:02:21 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <stdlib.h>

static const int	g_unclosed_quotes = -1;
static const bool	g_in_quotes = true;
static const bool	g_out_quotes = false;

/* helpers from tokenizer_word_utils.c */
bool	ft_hasspace(char *str);
char	*word_get_result(char *input, int len, bool in_quote);
void	eof_compute_gap(char *input, int *gap, char *quote);
t_token	*token_split_words(t_token *token, char *input, bool cat_to_next);

static void	set_token_value(
				t_token *token, char *input, int gap, int len, bool in_quote)
{
	char	*result;
	bool	cat_to_next;

	cat_to_next = false;
	if (input[gap + in_quote] && !ft_isspace(input[gap + in_quote]))
		cat_to_next = true;
	result = word_get_result(input, len, in_quote);
	if (!result)
	{
		perror("minishell:");
		set_exit_status(errno);
		exit_shell(NULL);
	}
	ezg_add(TOKEN, result);
	if (!in_quote && ft_hasspace(result))
		token_split_words(token, result, cat_to_next);
	else
	{
		token->cat_to_next = cat_to_next;
		token->value = result;
	}
}

static int	process_word_surrounded(t_token *token, char *input)
{
	int		gap;
	int		len;
	char	quote;

	gap = 1;
	len = 0;
	quote = input[0];
	while (input[gap] && input[gap] != quote)
	{
		len++;
		gap++;
	}
	if (input[gap] != quote)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: unexpected EOF while looking for matching `%c\n",
			quote);
		print_error("syntax error", "unexpected end of file");
		return (g_unclosed_quotes);
	}
	set_token_value(token, input, gap, len, g_in_quotes);
	gap++;
	return (gap);
}

int	process_word_nosurround(t_token *token, char *input)
{
	int		gap;
	int		len;

	gap = 0;
	len = 0;
	while (input[gap]
		&& !(input[gap] == '\''
			|| input[gap] == '"'
			|| ft_isspace(input[gap])
			|| input[gap] == '|'
			|| input[gap] == '<'
			|| input[gap] == '>'))
	{
		len++;
		gap++;
	}
	set_token_value(token, input, gap, len, g_out_quotes);
	return (gap);
}

int	fill_word_token(t_token *token, char *input)
{
	int		gap;
	int		spaces;

	spaces = 0;
	while (ft_isspace(*input))
	{
		spaces++;
		input++;
	}
	if (!input)
		return (spaces);
	gap = 0;
	if (input[0] == '\'' || input[0] == '"')
		gap = process_word_surrounded(token, input);
	else
		gap = process_word_nosurround(token, input);
	if (gap < 0)
		return (gap);
	token->type = WORD;
	return (gap + spaces);
}

int	fill_eof_token(t_token *token, char *input)
{
	int		gap;
	char	quote;

	eof_compute_gap(input, &gap, &quote);
	if (quote != 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: unexpected EOF while looking for matching `%c\n",
			quote);
		print_error("syntax error", "unexpected end of file");
		return (-1);
	}
	token->value = ezg_calloc(TOKEN, gap + 1, sizeof(char));
	if (!token->value)
	{
		perror("minishell:");
		set_exit_status(errno);
		exit_shell(NULL);
	}
	ft_strlcat(token->value, input, gap + 1);
	token->expand_dollar = false;
	token->type = WORD;
	token->cat_to_next = false;
	return (gap);
}