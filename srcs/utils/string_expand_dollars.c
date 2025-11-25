/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand_dollars.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 12:34:24 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/25 12:34:34 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezgalloc.h"
#include "ft_lib.h"
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

void	fill_chunks(char **splitted, char *str)
{
	int	i;
	int	len;
	int	n;

	i = 0;
	n = 0;
	while (str[i])
	{
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
		splitted[n++] = ft_substr(str, i, len);
		if (!splitted[n - 1])
		{
			splitted[n - 1] = ft_strdup("");
		}
		i += len;
	}
	splitted[n] = NULL;
}

static char	**ft_split_in_chunks(char *str, char delimiter)
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
		splitted = ft_calloc(delimiter_count * 2 + 1, sizeof(char *));
		fill_chunks(splitted, str);
	}
	return (splitted);
}

char	*string_expand_dollars(char *str)
{
	int												i;
	int												new_len;
	char											*new;
	char											*temp;
	char __attribute__	((cleanup(clean_array)))	**splitted;

	splitted = NULL;
	if (!ft_strchr(str, '$'))
		return (ft_strdup(str));
	splitted = ft_split_in_chunks(str, '$');
	new_len = 0;
	i = 0;
	i = 0;
	while (splitted[i])
	{
		if (splitted[i][0] == '$' && splitted[i][1])
		{
			if (splitted[i][1] == '?')
			{
				temp = ft_itoa(get_exit_status());
				free(splitted[i]);
				splitted[i] = temp;
			}
			else
			{
				temp = ft_getenv(&(splitted[i][1]));
				free(splitted[i]);
				if (temp)
					temp = ft_strdup(temp);
				else
					temp = ft_strdup("");
				splitted[i] = temp;
				temp = NULL;
			}
		}
		new_len += ft_strlen(splitted[i]);
		i++;
	}
	new = ft_calloc(new_len + 1, sizeof(char));
	i = 0;
	while (splitted[i])
	{
		ft_strlcat(new, splitted[i], new_len + 1);
		i++;
	}
	return (new);
}
