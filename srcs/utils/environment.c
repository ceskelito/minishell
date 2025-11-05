#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"
#include <stdlib.h>
#include <sys/types.h>

#define NOKEY	0
#define NOVALUE	0

enum { GET, SET, GET_ARRAY };


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

