/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:07:35 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 16:08:15 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <stdlib.h>

/* helpers from tokenizer_word_utils.c */
bool	ft_hasspace(char *str);
char	*word_get_result(char *input, int start, int len, bool expand);
void	eof_compute_gap(char *input, int *gap, char *quote);
t_token	*token_split_words(t_token *token, char *input, bool cat_to_next);

static void	set_token_value(
				t_token *token, char *input, int gap, bool in_quote)
{
	char	*result;
	bool	cat_to_next;
	int		len;
	int		start;

	cat_to_next = false;
	if (input[gap + in_quote] && !ft_isspace(input[gap + in_quote]))
		cat_to_next = true;
	if (in_quote)
	{
		start = 1;
		len = gap - 1;
	}
	else
	{
		start = 0;
		len = gap;
	}
	result = word_get_result(input, start, len, (input[0] != '\''));
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
	char	quote;

	gap = 1;
	quote = input[0];
	while (input[gap] && input[gap] != quote)
		gap++;
	if (input[gap] != quote)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: unexpected EOF while looking for matching `%c\n",
			quote);
		print_error("syntax error", "unexpected end of file");
		return (-1);
	}
	set_token_value(token, input, gap, true);
	gap++;
	return (gap);
}

int	process_word_nosurround(t_token *token, char *input)
{
	int	gap;

	gap = 0;
	while (input[gap]
		&& !(input[gap] == '\''
			|| input[gap] == '"'
			|| ft_isspace(input[gap])
			|| input[gap] == '|'
			|| input[gap] == '<'
			|| input[gap] == '>'))
		gap++;
	set_token_value(token, input, gap, false);
	return (gap);
}

int	fill_word_token(t_token *token, char *input)
{
	int	gap;
	int	spaces;

	spaces = 0;
	while (ft_isspace(*input))
	{
		spaces++;
		input++;
	}
	if (!input)
		return (spaces);
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
	token->type = WORD;
	token->cat_to_next = false;
	return (gap);
}
