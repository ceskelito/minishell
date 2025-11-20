/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:23:22 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/20 16:25:00 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

static bool	is_entry_valid(char *entry)
{
	int	i;

	if (!ft_isalpha(entry[0]))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", entry);
		return (false);
	}
	i = 0;
	while (entry[i] && entry[i] != '=')
	{
		if (!ft_isalnum(entry[i]))
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: export: `%c': not a valid identifier\n", entry);
			return (false);
		}
		i++;
	}
	if (!ft_strchr(entry, '='))
		return (false);
	return (true);
}

void	export(char *const args[])
{
	size_t	key_len;
	int		i;
	char	*key;
	char	*value;
	char	*entry;

	i = 0;
	while (i++, args[i])
	{
		entry = args[i];
		if (!is_entry_valid(entry))
			continue ;
		key_len = 0;
		while (entry[key_len] != '=')
			key_len++;
		key = ft_substr(entry, 0, key_len);
		value = ft_strchr(entry, '=') + 1;
		ft_setenv(key, value);
		free(key);
	}
}

void	unset(char *const args[])
{
	int	i;

	i = 0;
	while (i++, args[i])
	{
		ft_unsetenv(args[i]);
	}
}

void	env(void)
{
	char	**env_array;
	int		i;

	env_array = ft_getenv_array();
	i = 0;
	while (env_array[i])
	{
		ft_printf("%s\n", env_array[i]);
		i++;
	}
}
