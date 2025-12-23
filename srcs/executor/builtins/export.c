/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:50:13 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 16:27:11 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strdup_prevchr(char *entry, char delimiter)
{
	size_t	key_len;
	char	*key;

	key_len = 0;
	while (entry[key_len] != delimiter)
		key_len++;
	key = ft_substr(entry, 0, key_len);
	return (key);
}

static bool	is_entry_valid(char *entry, int *had_error)
{
	int	i;

	if (!ft_isalpha(entry[0]) && entry[0] != '_')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", entry);
		*had_error = 1;
		return (false);
	}
	i = 0;
	while (entry[i] && entry[i] != '=')
	{
		if (!ft_isalnum(entry[i]) && entry[i] != '_')
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: export: `%s': not a valid identifier\n", entry);
			*had_error = 1;
			return (false);
		}
		i++;
	}
	if (!ft_strchr(entry, '='))
		return (false);
	return (true);
}

static void	sort_strings(char *array[])
{
	char	*temp;
	int		i;
	int		size;
	int		j;

	i = 0;
	size = 0;
	while (array && array[size])
		size++;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strcmp(array[i], array[j]) > 0)
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_export(void)
{
	char											*key;
	char											*value;
	int												i;
	char __attribute__	((cleanup(clean_array)))	**dest;

	dest = dup_array(NULL, ft_getenv_array());
	if (!dest)
		return ;
	sort_strings(dest);
	i = 0;
	while (dest[i])
	{
		key = ft_strdup_prevchr(dest[i], '=');
		value = ft_strchr(dest[i], '=');
		printf("declare -x \"%s%s\"\n", key, value + 1);
		free(key);
		i++;
	}
}

void	export(char *const args[])
{
	int		i;
	int		exit_code;
	char	*key;
	char	*value;
	char	*entry;

	exit_code = 0;
	if (!args || !args[1])
	{
		print_export();
		set_exit_status(0);
		return ;
	}
	i = 0;
	while (i++, args[i])
	{
		entry = args[i];
		if (!is_entry_valid(entry, &exit_code))
			continue ;
		key = ft_strdup_prevchr(entry, '=');
		value = ft_strchr(entry, '=') + 1;
		ft_setenv(key, value);
		free(key);
	}
	set_exit_status(exit_code);
}
