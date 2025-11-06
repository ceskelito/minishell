/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:39:49 by ceskelito         #+#    #+#             */
/*   Updated: 2025/11/06 12:42:34 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"

/**
 * change_variable_value - Replace the value part of a "key=value" string.
 *
 * This helper preserves the key portion and rebuilds the string using the
 * provided new value. The old string is released and a new one is allocated
 * in the ENV arena.
 *
 * @variable  Existing "key=value" string to update.
 * @new_value New value to assign to the key.
 *
 * Return: Nothing.
 */
static void	change_variable_value(char *variable, char *new_value)
{
	int		i;
	char	**tmp;

	tmp = ft_split(variable, '=');
	if (!tmp || !tmp[0])
		return ;
	ezg_release(ENV, variable);
	variable = ezg_calloc(ENV, sizeof(char), ft_strlen(tmp[0]) + ft_strlen(new_value) + 2);
	ft_sprintf(variable, "%s=%s\0", tmp[0], new_value);
	i = 0;
	while (tmp[i])
	{
		free(tmp[i]);
		tmp++;
	}
	free(tmp);
}

/**
 * add_variable - Append a new "key=value" entry to the environment array.
 *
 * Ensures there is room for one more entry (plus the terminating NULL),
 * builds the "key=value" string and inserts it at the end of the array.
 * The @env pointer may be reallocated and updated in place.
 *
 * @env   Address of the environment array to extend.
 * @key   Variable name to add.
 * @value Value to assign to the new variable.
 *
 * Return: Nothing.
 */
static void add_variable(char ***env, char *key, char *value)
{
    int nmemb;

    nmemb = 0;
	while ((*env)[nmemb])
		nmemb++;
	*env = expand_array(ENV, *env, nmemb, 1);
	if (!*env)
		return (NULL);
	*env[nmemb] = ezg_calloc(ENV, sizeof(char), ft_strlen(key) + ft_strlen(value) + 2);
	ft_sprintf(*env[nmemb], "%s=%s", key, value);
	*env[nmemb + 1] = NULL;
}

/**
 * get_variable - Find an entry by key in a "key=value" string array.
 *
 * Performs a linear search comparing the key up to the '=' separator.
 *
 * @env Array of "key=value" strings.
 * @key Name to look up.
 *
 * Return: Address of the matching entry within @env on success, NULL on failure.
 */
static char **get_variable(char **env, char *key)
{
    int i;
    int j;

    i = 0;
	while (env && env[i])
	{
		j = 0;
		while (key[j] && env[i][j] && key[j] == env[i][j])
			j++;
		if (!key[j] && env[i][j] == '=')
			return (&env[i]);
		i++;
	}
	return (NULL);
}

/**
 * env_handler - Internal environment handler for key=value string arrays.
 *
 * This function provides centralized management for environment variables
 * stored as a static array of "key=value" strings. It supports three modes:
 *   GET        - Search for an entry matching @key and return a pointer to it.
 *   SET        - Update the value of @key if it exists, otherwise append a new entry.
 *   GET_ARRAY  - Return a pointer to the full environment array.
 *
 * @mode   Operation mode (GET, SET, or GET_ARRAY).
 * @key    Variable name to retrieve or modify. Ignored if mode is GET_ARRAY.
 * @value  New value to assign when using SET mode.
 *
 * Return: A pointer to:
 *         - The matching environment string (GET),
 *         - The environment array (GET_ARRAY),
 *         - NULL on error or if the key was not found.
 */
char **env_handler(int mode, char *key, char *value)
{
	static char	**env;
	extern char	**environ;
	char		**temp;
	int			i;
	int			j;

	if (!env)
		env = dup_array(ENV, environ);
	if (mode == GET)
	{
		return (get_variable(env, key));
	}
	else if (mode == SET)
	{
		temp = env_handler(GET, key, NOVALUE);
		if (temp)
			change_variable_value(*temp, value);
        else
            add_variable(&env, key, value);
	}
	else if (mode == GET_ARRAY)
	{
		return (env);
	}
	return (NULL);
}
