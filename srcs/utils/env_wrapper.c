#include "minishell.h"

/*
 * env_get - Get environment variable value
 * @key: Variable name
 * Return: Variable value or NULL if not found
 */
char	*env_get(char *key)
{
	char	*value;

	if (!key)
		return (NULL);
	
	value = getenv(key);
	return (value);
}

/*
 * env_get_safe - Get environment variable with default value
 * @key: Variable name
 * @default_val: Default value if variable not found
 * Return: Variable value or default value
 */
char	*env_get_safe(char *key, char *default_val)
{
	char	*value;

	value = env_get(key);
	if (!value)
		return (default_val);
	
	return (value);
}

/*
 * env_set - Set environment variable
 * @key: Variable name
 * @value: Variable value
 * Return: 0 on success, -1 on failure
 */
int	env_set(char *key, char *value)
{
	if (!key)
	{
		handle_internal_error("env_set", "key is NULL");
		return (-1);
	}
	
	if (setenv(key, value ? value : "", 1) == -1)
	{
		handle_system_error("setenv", key);
		return (-1);
	}
	
	return (0);
}

/*
 * env_unset - Remove environment variable
 * @key: Variable name to remove
 * Return: 0 on success, -1 on failure
 */
int	env_unset(char *key)
{
	if (!key)
	{
		handle_internal_error("env_unset", "key is NULL");
		return (-1);
	}
	
	if (unsetenv(key) == -1)
	{
		handle_system_error("unsetenv", key);
		return (-1);
	}
	
	return (0);
}

/*
 * env_exists - Check if environment variable exists
 * @key: Variable name to check
 * Return: true if exists, false otherwise
 */
bool	env_exists(char *key)
{
	return (env_get(key) != NULL);
}

/*
 * validate_env_name - Validate environment variable name
 * @name: Variable name to validate
 * Return: true if valid, false otherwise
 */
bool	validate_env_name(char *name)
{
	int	i;

	if (!name || !name[0])
		return (false);
	
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (false);
	
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	
	return (true);
}

/*
 * env_export - Export variable (with validation)
 * @assignment: String in format "KEY=VALUE" or just "KEY"
 * Return: 0 on success, -1 on failure
 */
int	env_export(char *assignment)
{
	char	*equal_pos;
	char	*key;
	char	*value;

	if (!assignment)
		return (-1);
	
	equal_pos = ft_strchr(assignment, '=');
	if (equal_pos)
	{
		key = ezg_add(EXECUTING, ft_substr(assignment, 0, equal_pos - assignment));
		value = equal_pos + 1;
	}
	else
	{
		key = assignment;
		value = "";
	}
	
	if (!validate_env_name(key))
	{
		ft_dprintf(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", assignment);
		set_exit_status(1);
		return (-1);
	}
	
	return (env_set(key, value));
}

/*
 * env_print_all - Print all environment variables
 */
void	env_print_all(void)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}