/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_in_chunks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 12:52:38 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/12 11:58:21 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_char(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c)
			count++;
		i++;
	}
	return (count);
}

static int	get_chunk_len(char *str, int i)
{
	int	len;

	len = 0;
	while (str[i + len] && str[i + len] != '$')
		len++;
	if (len == 0 && str[i] == '$')
	{
		len = 1;
		if (str[i + len] == '?')
			len++;
		else
			while (str[i + len] && (ft_isalnum(str[i + len])
					|| str[i + len] == '_'))
				len++;
	}
	return (len);
}

void	fill_chunks(char **splitted, char *str)
{
	int	i;
	int	len;
	int	n;

	i = 0;
	n = 0;
	while (str[i])
	{
		len = get_chunk_len(str, i);
		splitted[n] = ft_substr(str, i, len);
		splitted[n + 1] = NULL;
		if (!splitted[n])
			splitted[n] = ft_strdup("");
		n++;
		i += len;
	}
}

char	**ft_split_in_chunks(char *str, char delimiter)
{
	int		delimiter_count;
	char	**splitted;

	delimiter_count = count_char(str, delimiter);
	if (!delimiter_count)
	{
		splitted = ft_calloc(2, sizeof(char *));
		splitted[0] = ft_strdup(str);
		splitted[1] = NULL;
	}
	else
	{
		splitted = ft_calloc(delimiter_count * 2 + 2, sizeof(char *));
		fill_chunks(splitted, str);
	}
	return (splitted);
}
