/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:01:11 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/11 16:08:01 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_char_safe(char *str, char c)
{
	char	*temp;

	temp = ft_strjoin_char(str, c);
	if (!temp)
		return (NULL);
	free(str);
	return (temp);
}

char	*if_nclsd_qts(char quote_char, char *result, const char *type)
{
	if (quote_char != type[0])
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error: unclosed %s\n", type);
		if (result)
			free(result);
		return (NULL);
	}
	return (result);
}

char	*handle_single_quote(char *input, int *i, char *result)
{
	(*i)++;
	while (input[*i] && input[*i] != '\'')
	{
		result = append_char_safe(result, input[*i]);
		if (!result)
			return (NULL);
		(*i)++;
	}
	result = if_nclsd_qts(input[*i], result, "'");
	if (result)
		(*i)++;
	return (result);
}

char	*process_quotes(char *input, int *i, char *result)
{
	char	quote;

	quote = input[*i];
	if (quote == '\'')
		return (handle_single_quote(input, i, result));
	if (quote == '\"')
		return (handle_double_quote(input, i, result));
	return (result);
}
