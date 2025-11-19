/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:39:49 by ceskelito         #+#    #+#             */
/*   Updated: 2025/11/19 12:48:36 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lib.h"
#include "minishell.h"
#include "environment.h"

/**
 * change_entry_value - Safely replace the value of an existing env entry.
 *
 * Preserves the key portion of the entry and change the value.
 * The old string is released and a new one is allocated in the ENV arena.
 *
 * @entry_ptr Pointer to an existing entry to update.
 * @new_value New value to assign to the entry.
 *
 * Return: Nothing.
 */
static void	change_entry_value(char **entry_ptr, char *new_value)
{
	size_t	key_len;
	size_t	value_len;
	char	*old_entry;
	char	*new_entry;

	if (!entry_ptr || !*entry_ptr)
		return ;
	old_entry = *entry_ptr;
	key_len = 0;
	while (old_entry[key_len] != '=')
		key_len++;
	value_len = ft_strlen(new_value);
	new_entry = ezg_calloc(ENV, sizeof(char), key_len + value_len + 2);
	if (!new_entry)
		return ;
	ft_strlcpy(new_entry, old_entry, key_len + 1);
	new_entry[key_len] = '=';
	ft_strlcat(&new_entry[key_len], new_value, value_len + 2);
	ezg_release(ENV, old_entry);
	*entry_ptr = new_entry;
}

/**
 * add_entry - Append a new "key=value" entry to the environment array.
 *
 * Ensures space for the new entry plus the terminating NULL,
 * builds the "key=value" string and inserts it at the end of the array.
 * The @env pointer may be reallocated and updated in place.
 *
 * @env   Address of the environment array to extend.
 * @key   Variable name.
 * @value Variable value.
 *
 * Return: Nothing.
 */
static void add_entry(char ***env, char *key, char *value)
{
    size_t	nmemb;
	char	*kv;

	if (!env || !*env || !key)
        return;
	kv = ezg_calloc(ENV, sizeof(char), ft_strlen(key) + ft_strlen(value) + 2);
	if (!kv)
        return;
	ft_sprintf(kv, "%s=%s", key, value);
    nmemb = 0;
	while ((*env)[nmemb])
		nmemb++;
	*env = expand_array(ENV, *env, nmemb, 1);
	if (!*env)
		return ;
	(*env)[nmemb] = kv;
	(*env)[nmemb + 1] = NULL;
}

static void	remove_entry(char ***env, char *entry)
{
    char	**arr;
    int		i;

    if (!env || !*env || !entry)
        return ;
    arr = *env;
    i = 0;
    while (arr[i] && arr[i] != entry)
        i++;
    if (!arr[i])
        return ;
    ezg_release(ENV, arr[i]);
    while (arr[i + 1])
    {
        arr[i] = arr[i + 1];
        i++;
    }
    arr[i] = NULL;
}


/**
 * get_entry - Locate an entry by key.
 *
 * Linear search comparing characters up to '='. Returns the address of the
 * slot (&env[i]), not the string itself, to allow in-place modification.
 *
 * @env Environment array.
 * @key Key to search (must be non-NULL).
 *
 * Return: &env[i] on success, NULL on failure.
 */
static char **get_entry(char **env, char *key)
{
    int i;
    int j;

    i = 0;
	while (env && env[i])
	{
		j = 0;
		// while (ft_isspace(*key))
			// key++;
		while (key[j] && env[i][j] && key[j] == env[i][j])
			j++;
		if ((!key[j] /*|| ft_isspace(key[j])*/) && env[i][j] == '=')
			return (&env[i]);
		i++;
	}
	return (NULL);
}

/**
 * env_handler - Internal handler for the environment strings array.
 *
 * This function provides centralized management for environment variables
 * stored as a static array of "key=value" strings. It supports three modes:
 *   GET        - Search for an entry matching @key and return a pointer to it.
 *   SET        - Update the value of @key if it exists, otherwise append a new entry.
 *   GET_ARRAY  - Return the full environment array.
 *
 * @mode   Operation mode (GET, SET, or GET_ARRAY).
 * @key    Variable name to retrieve or modify. Ignored if mode is GET_ARRAY.
 * @value  New value to assign when using SET mode.
 *
 * Return: A pointer to:
 *         - The matching entry (GET),	
 *         - The environment array (GET_ARRAY),
 * 		   - NULL (SET),
 *         - NULL on error or if the key was not found.
 */
char **env_handler(int mode, char *key, char *value)
{
	static char	**env;
	extern char	**environ;
	char		**entry;

	if (!env)
		env = dup_array(ENV, environ);
	if (mode == GET)
		return (get_entry(env, key));
	else if (mode == SET)
	{
		entry = get_entry(env, key);
		if (entry)
			change_entry_value(entry, value);
		else
			add_entry(&env, key, value);
	}
	else if (mode == UNSET)
	{
		entry = get_entry(env, key);
		if (entry)
			remove_entry(&env, *entry);
	}
	else if (mode == GET_ARRAY)
		return (env);
	return (NULL);
}
