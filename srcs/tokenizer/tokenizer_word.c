/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 16:07:35 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/06 13:59:35 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <stdlib.h>

static const int	UNCLOSED_QUOTES = -1;
static const bool	IN_QUOTES = true;
static const bool	OUT_QUOTES = false;

static inline bool	isquote(char c)
{
	return (c == '\'' || c == '"');	
}

static inline bool isspecial(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (true);
	return (false);
}

static inline bool ft_hasspace(char *str)
{
	int i;

	i = 0;
	while(str && str[i])
	{
		if (ft_isspace(str[i]))
			return (true);
		i++;
	}
	return (false);
}

t_token	*new_token();

static t_token	*token_split_words(t_token *token, char *input, bool cat_to_next)
{
	int		i;
	char	**split;
	t_token	*curr;

	split = ft_split_func(input, ft_isspace);
	if (!split)
	{
		perror("minishell");
		set_exit_status(errno);
		exit_shell(NULL);
	}
	i = 0;
	while (split[i])
	{
		if (i == 0)
			curr = token;
		else
		{
			curr->next = new_token();
			curr = curr->next;
		}
		curr->type = WORD;
		curr->value = split[i];
		ezg_add(TOKEN, curr->value);
		i++;
	}
	curr->cat_to_next = cat_to_next;
	free(split);
	return (token);
}

static void	set_token_value(t_token *token, char *input, int gap, int len, bool in_quote)
{
	char	*tmp;
	char	*result;
	bool	cat_to_next;

	cat_to_next = false;
	if (input[gap + in_quote] && !ft_isspace(input[gap + in_quote]))
		cat_to_next = true;
	if (len == 0)
		result = ft_strdup("");
	else if (input[0] != '\'')
	{
		tmp = ft_substr(input, in_quote, len);
		result = string_expand_dollars(tmp);
		free(tmp);
	}	
	else
		result = ft_substr(input, in_quote, len);
	if (!result)
	{
		perror("minishell:");
		set_exit_status(errno);
		exit_shell(NULL);
	}
	ezg_add(TOKEN, result);
	if (!in_quote && ft_hasspace(result))
	{
		token_split_words(token, result, cat_to_next);
		//free(result);
	}
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
		ft_dprintf(STDERR_FILENO, "minishell: unexpected EOF while looking for matching `%c\n", quote);
		print_error("syntax error", "unexpected end of file");
		return (UNCLOSED_QUOTES);
	}
	set_token_value(token, input, gap, len, IN_QUOTES);
	gap++;
	return (gap);
}

int	process_word_nosurround(t_token *token, char *input)
{
	int		gap;
	int		len;

	gap = 0;
	len = 0;
	while (input[gap] && !(isquote(input[gap]) || ft_isspace(input[gap]) || isspecial(input[gap])))
	{
		len++;
		gap++;
	}
	set_token_value(token, input, gap, len, OUT_QUOTES);
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
	if (isquote(input[0]))
		gap = process_word_surrounded(token, input);
	else
		gap = process_word_nosurround(token, input);
	if (gap < 0)
		return (gap);
	token->type = WORD;
	return (gap + spaces);
}

int fill_eof_token(t_token *new, char *input)
{
	int		i;
	int		gap;
	char	quote;


	i = 0;
	gap = 0;
	quote = 0;
	while(input[i] && !ft_isspace(input[i])
		&& !ft_strchr("|<>", input[i]))
	{
		if (quote == 0 && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		i++;
		gap++;
	}
	if (quote != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: unexpected EOF while looking for matching `%c\n", quote);
		print_error("syntax error", "unexpected end of file");
		return (-1);
	}
	new->value = ezg_calloc(TOKEN, gap + 1, sizeof(char));
	ft_strlcat(new->value, input, gap + 1);
	if (!new->value)
	{
		perror("minishell:");
		set_exit_status(errno);
		exit_shell(NULL);
	}
	new->expand_dollar = false;
	new->type = WORD;
	new->cat_to_next = false;
	return (gap);
}
