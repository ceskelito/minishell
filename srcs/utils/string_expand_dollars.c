/* ************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand_dollars.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:15:36 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/19 16:40:38 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"

static int	count_char(char *s, char c)
{
	int		i;
	int		count;

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
	int		i;
	int		len;
	int		n;

	i = 0;
	n = 0;
	while (len = 0, str[i])
	{
		while (str[i + len] && str[i + len] != '$')
			len++;
		if (len == 0 && str[i] == '$')
		{
			len = 1;
			if (str[i + len] == '?')
				len++;
			else
				while (str[i + len] && (ft_isalnum(str[i + len]) || str[i + len] == '_'))
					len++;
		}
		splitted[n++] = ft_substr(str, i, len);
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
	}
	else
	{
		splitted = ft_calloc(delimiter_count * 2 + 1, sizeof(char *));
		fill_chunks(splitted, str);
	}
	return (splitted);
}

static void	string_collapse_spaces(char **str)
{
	char	*old;
	char	*new;
	int		i;
	int		j;

	old = *str;
	new = ft_calloc(ft_strlen(old) + 1, sizeof(char));
	if (!new)
	{
		*str = NULL;
		return ;
	}
	i = 0;
	j = 0;
	while (old[i])
	{
		if (ft_isspace(old[i]))
		{
			new[j++] = ' ';
			while (ft_isspace(old[i]))
				i++;
		}
		else
			new[j++] = old[i++];
	}
	new[j] = '\0';
	free(old);
	*str = new;
}

char	*string_expand_dollars(char *str, bool collapse_spaces)
{
	int												i;
	int												new_len;
	char											*new;
	char											*temp;
	char											**splitted;
	// char __attribute__((cleanup(clean_array)))		**splitted;

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
				/* Environment variable name starts at index 1 */
				temp = ft_getenv(&(splitted[i][1]));
				free(splitted[i]);
				if (temp)
					temp = ft_strdup(temp);
				else
					temp = ft_strdup("");
				if (collapse_spaces)
					string_collapse_spaces(&temp);
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
	/* Cleanup chunks */
	i = 0;
	while (splitted[i])
		free(splitted[i++]);
	free(splitted);
	return (new);
}
