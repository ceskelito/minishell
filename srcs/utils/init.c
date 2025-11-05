#include "minishell.h"

void	init_shell(t_shell *shell)
{
	shell->env_list = NULL;
	shell->cmd_list = NULL;
	shell->tokens = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	shell->interactive = 1;
	shell->std_out = dup(STDOUT_FILENO);
	shell->std_in = dup(STDIN_FILENO);
}

/**
 * dup_array - Duplicate the system environment into the ezgalloc-managed ENV group.
 *
 * This function copies all entries from the provided @environ array into
 * a new NULL-terminated array allocated under the ezgalloc group ENV.
 * The resulting array replaces the one referenced by ft_getenv_array(),
 * making it available for later access and manipulation through the
 * environment utility functions defined in utils/environment.c.
 *
 * @environ  Pointer to the system environment array (NULL-terminated).
 *
 * Return: None.
 */
char	**dup_array(char **environ)
{
	int		i;
	int		nmemb;
	char	**env;

	nmemb = 0;
	while (environ[nmemb])
		nmemb++;
	env = ezg_calloc(ENV, sizeof(char *), nmemb + 1);
	i = 0;
	while (environ[i])
	{
		env[i] = ft_strdup(environ[i]);
		ezg_add(ENV, env[i]);
		i++;
	}
    env[i] = NULL;
	return (env);
}