/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:01:11 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/11 16:08:01 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_char_safe(char *str, char c);
char	*if_nclsd_qts(char quote_char, char *result, const char *type);

static char	*handle_dollar_in_quote(char *input, int *i, char **result)
{
	char	*temp;

	temp = *result;
	handle_dollar_sign(input, i, result);
	if (!*result)
	{
		if (temp)
			free(temp);
		return (NULL);
	}
	return (*result);
}

static char	*process_dquote_char(char *input, int *i, char *result)
{
	if (input[*i] == '$')
		return (handle_dollar_in_quote(input, i, &result));
	result = append_char_safe(result, input[*i]);
	if (!result)
		return (NULL);
	(*i)++;
	return (result);
}

char	*handle_double_quote(char *input, int *i, char *result)
{
	(*i)++;
	while (input[*i] && input[*i] != '\"')
	{
		result = process_dquote_char(input, i, result);
		if (!result)
			return (NULL);
	}
	result = if_nclsd_qts(input[*i], result, "\"");
	if (result)
		(*i)++;
	return (result);
}
