/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:31:54 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/16 19:31:56 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(void);

bool	ft_hasspace(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (ft_isspace(str[i]))
			return (true);
		i++;
	}
	return (false);
}

char	*word_get_result(char *input, int start, int len, bool expand)
{
	char	*tmp;
	char	*result;

	if (len <= 0)
		return (ft_strdup(""));
	if (!expand)
		return (ft_substr(input, start, len));
	tmp = ft_substr(input, start, len);
	if (!tmp)
		return (NULL);
	result = string_expand_dollars(tmp);
	free(tmp);
	return (result);
}

void	eof_compute_gap(char *input, int *gap, char *quote)
{
	int	i;

	i = 0;
	*gap = 0;
	*quote = 0;
	while (input[i] && !ft_isspace(input[i])
		&& !ft_strchr("|<>", input[i]))
	{
		if (*quote == 0 && (input[i] == '\'' || input[i] == '"'))
			*quote = input[i];
		else if (input[i] == *quote)
			*quote = 0;
		i++;
		(*gap)++;
	}
}

static void	fill_split_tokens(char **split, t_token *token, bool cat_to_next)
{
	t_token	*curr;
	int		i;

	curr = token;
	i = 0;
	while (split[i])
	{
		if (i != 0)
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
}

t_token	*token_split_words(t_token *token, char *input, bool cat_to_next)
{
	char	**split;

	split = ft_split_func(input, ft_isspace);
	if (!split)
	{
		perror("minishell");
		set_exit_status(1);
		exit_shell(NULL);
	}
	fill_split_tokens(split, token, cat_to_next);
	free(split);
	return (token);
}
