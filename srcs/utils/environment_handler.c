
#include "minishell.h"
#include "environment.h"

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
