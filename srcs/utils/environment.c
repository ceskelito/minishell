#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"
#include <stdlib.h>
#include <sys/types.h>

#define NOKEY	0
#define NOVALUE	0

enum { GET, SET, GET_ARRAY };

/**
 * expand_array - Expand or shrink a dynamically allocated NULL-terminated array of strings.
 *
 * This function reallocates the given array by the specified increment.
 * If @increment is positive, the array is expanded; if negative, it is reduced.
 * If the resulting size is zero or negative, the entire array and its contents are released.
 *
 * @group      Identifier of the ezgalloc memory group used for allocation.
 * @array      Pointer to the NULL-terminated array to be modified.
 * @nmemb      Number of elements currently stored in the array.
 * @increment  Number of elements to add (positive) or remove (negative).
 *
 * Return: A pointer to the new array if successful, or NULL if the array
 *         was released or allocation failed.
 */
static char	**expand_array(char *group, char **array, int nmemb, int increment)
{
	char	**new;

	if (!array)
		return (NULL);
	if (increment == 0)
		return (array);
	if ((nmemb + increment) <= 0)
	{
		while (nmemb-- > 0)
			ezg_release(group, array[nmemb]);
		ezg_release(group, array);
		return (NULL);
	}
	new = ezg_calloc(group, sizeof(char *), nmemb + increment + 1);
	if (!new)
		return (array);
	ft_memcpy(new, array, nmemb * sizeof(char *));
	ezg_release(group, array);
	return (new);
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
static char **env_handler(int mode, char *key, char *value)
{
	static char	**env;
	extern char	**environ;
	int			nmemb;
	int			i;
	int			j;

	if (!env)
		env = dup_array(environ);
	if (mode == GET)
	{
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
	else if (mode == SET) // NEED TO CHECK IF A VARIABLE ALREADY EXISTS
	{
		nmemb = 0;
		while (env[nmemb])
			nmemb++;
		env = expand_array(ENV, env, nmemb, 1);
		if (!env)
			return (NULL);
		env[nmemb] = ezg_calloc(ENV, sizeof(char), ft_strlen(key) + ft_strlen(value) + 2);
		ft_sprintf(env[nmemb], "%s=%s", key, value);
		env[nmemb + 1] = NULL;
	}
	else if (mode == GET_ARRAY)
	{
		return (env);
	}
	return (NULL);
}

/* void ft_setenv_array()
{
	env_handler(SET_ARRAY, NOKEY, NOVALUE);
} */

char **ft_getenv_array()
{
	return (env_handler(GET_ARRAY, NOKEY, NOVALUE));
}

void	ft_setenv(char *key, char *value)
{
	if (!key)
		return ;
	if (!value)
		value = "";
	env_handler(SET, key, value);
}

char	*ft_getenv(char *key)
{
	char	**tmp;

	if (!key)
		return (NULL);
	tmp = env_handler(GET, key, NOVALUE);
	if (!tmp)
		return (NULL);
	return (ft_strchr(*tmp, '=') + 1);
}

