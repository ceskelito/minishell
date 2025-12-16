/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand_dollars.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 12:34:24 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/12 11:57:24 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_split_in_chunks(char *str, char delimiter);

static char	*expand_exit_status(char *chunk)
{
	char	*temp;

	temp = ft_itoa(get_exit_status());
	free(chunk);
	return (temp);
}

static char	*expand_env_var(char *chunk)
{
	char	*temp;

	temp = ft_getenv(&(chunk[1]));
	free(chunk);
	if (temp)
		temp = ft_strdup(temp);
	else
		temp = ft_strdup("");
	return (temp);
}

static void	expand_chunk(char **splitted, int i)
{
	if (splitted[i][0] == '$' && splitted[i][1])
	{
		if (splitted[i][1] == '?')
			splitted[i] = expand_exit_status(splitted[i]);
		else
			splitted[i] = expand_env_var(splitted[i]);
	}
}

static int	expand_all_chunks(char **splitted)
{
	int	i;
	int	new_len;

	i = 0;
	new_len = 0;
	while (splitted[i])
	{
		expand_chunk(splitted, i);
		new_len += ft_strlen(splitted[i]);
		i++;
	}
	return (new_len);
}

char	*string_expand_dollars(char *str)
{
	int												i;
	int												new_len;
	char											*new;
	char __attribute__	((cleanup(clean_array)))	**splitted;

	splitted = NULL;
	if (!ft_strchr(str, '$'))
		return (ft_strdup(str));
	splitted = ft_split_in_chunks(str, '$');
	new_len = expand_all_chunks(splitted);
	new = ft_calloc(new_len + 1, sizeof(char));
	i = 0;
	while (splitted[i])
	{
		ft_strlcat(new, splitted[i], new_len + 1);
		i++;
	}
	return (new);
}
