/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:31:54 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/16 19:31:56 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_hasspace(char *str);
char	*word_get_result(char *input, int start, int len, bool expand);
void	eof_compute_gap(char *input, int *gap, char *quote);
t_token	*token_split_words(t_token *token, char *input, bool cat_to_next);

static bool	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	set_value_params(t_token_params *p, char *input, int gap, bool q)
{
	if (q)
	{
		p->start = 1;
		p->len = gap - 1;
	}
	else
	{
		p->start = 0;
		p->len = gap;
	}
	p->cat_to_next = false;
	if (input[gap + q] && !ft_isspace(input[gap + q])
		&& !is_operator_char(input[gap + q]))
		p->cat_to_next = true;
}

void	set_token_value(t_token *token, char *input, int gap, bool q)
{
	char			*result;
	t_token_params	p;

	set_value_params(&p, input, gap, q);
	result = word_get_result(input, p.start, p.len, (input[0] != '\''));
	if (!result)
	{
		perror("minishell:");
		set_exit_status(errno);
		exit_shell(NULL);
	}
	ezg_add(TOKEN, result);
	if (!q && ft_hasspace(result))
		token_split_words(token, result, p.cat_to_next);
	else
	{
		token->cat_to_next = p.cat_to_next;
		token->value = result;
	}
}

int	process_word_nosurround(t_token *token, char *input)
{
	int	gap;

	gap = 0;
	while (input[gap] && !(input[gap] == '\'' || input[gap] == '"'
			|| ft_isspace(input[gap]) || input[gap] == '|'
			|| input[gap] == '<' || input[gap] == '>'))
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
