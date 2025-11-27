/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 11:50:13 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/27 11:50:16 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

static char *ft_strdup_prevchr(char *entry, char delimiter)
{
    size_t  key_len;
    char    *key;

    key_len = 0;
	while (entry[key_len] != delimiter)
		key_len++;
	key = ft_substr(entry, 0, key_len);
    return (key);
}

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

static void sort_strings(char *array[])
{
    char *temp;
    int i = 0;
    int size;

    size = 0;
	while (array && array[size])
	    size++;
    while (i < size - 1) {
        int j = i + 1;
        
        while (j < size) {
            if (ft_strcmp(array[i], array[j]) > 0) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
            j++;
        }
        i++;
    }
}

void    print_export()
{
    char __attribute__  ((cleanup(clean_array)))    **dest;
    char                                            *key;
    char                                            *value;
    int                                             i;

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
	char	*key;
	char	*value;
	char	*entry;

	if (!args || !args[1])
	{
		print_export();
		return ;
	}
	i = 0;
	while (i++, args[i])
	{
		entry = args[i];
		if (!is_entry_valid(entry))
			continue ;
		key = ft_strdup_prevchr(entry, '=');
		value = ft_strchr(entry, '=') + 1;
		ft_setenv(key, value);
		free(key);
	}
}