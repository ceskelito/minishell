/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:12:31 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/08 17:12:43 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_char_safe(char *str, char c)
{
	char	*temp;

	temp = ft_strjoin_char(str, c);
	if (!temp)
		return (NULL);
	free(str);
	return (temp);
}

static char	*handle_single_quote(char *input, int *i, char *result)
{
	(*i)++;
	while (input[*i] && input[*i] != '\'')
	{
		result = append_char_safe(result, input[*i]);
		if (!result)
			return (NULL);
		(*i)++;
	}
	return (if_nclsd_qts(input[*i], result, "'"));
}

static char	*handle_double_quote(char *input, int *i, char *result)
{
	char	*temp;

	(*i)++;
	while (input[*i] && input[*i] != '\"')
	{
		if (input[*i] == '$')
		{
			temp = result;
			handle_dollar_sign(input, i, &result);
			if (!result)
			{
				if (temp)
					free(temp);
				return (NULL);
			}
		}
		else
		{
			result = append_char_safe(result, input[*i]);
			if (!result)
				return (NULL);
			(*i)++;
		}
	}
	return (if_nclsd_qts(input[*i], result, "\""));
}

static char	*if_nclsd_qts(char quote_char, char *result, const char *quote_type)
{
	if (quote_char != quote_type[0])
	{
		printf("minishell: syntax error: unclosed %s\n", quote_type);
		if (result)
			free(result);
		return (NULL);
	}
	return (result);
}

char	*process_quotes(char *input, int *i, char *result)
{
	char	quote;

	quote = input[*i];
	if (quote == '\'')
		return (handle_single_quote(input, i, result));
	else if (quote == '\"')
		return (handle_double_quote(input, i, result));
	return (result);
}
