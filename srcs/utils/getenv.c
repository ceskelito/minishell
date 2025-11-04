#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"

// #define GET 1
// #define SET 2

// void	expand_array(char ***array, int *count)
// {
// 	int		i;
// 	char	**new;

// 	new = ezg_calloc(GLOBAL, sizeof(char *), *count + 1);
// 	i = 0;
// 	while (i < *count)
// 	{
// 		new[i] = (*array)[i];
// 		i++;
// 	}
// 	new[i + 1] = NULL;
// 	new[i + 2] = NULL;
// 	ezg_release(GLOBAL, (*array));
// 	(*array) = new;
// 	(*count) += 1;
// 	new = NULL;
// }

// char	*get_value(char **array, char *key)
// {
// 	return (NULL);
// }

// char	*env_handler(int mode, char *key, char *value)
// {
// 	static char	**env;
// 	static int	count;

// 	if (mode == SET)
// 	{
// 		expand_array(&env, &count);
// 		env[count - 1] = ezg_calloc(GLOBAL, sizeof(char),
// 		                    ft_strlen(key) + ft_strlen(value) + 2);
// 		ft_sprintf(env[count], "%s=%s\0", key, value);
// 	}
// 	else if (mode == GET)	
// 	{
// 		return (get_value(env, key));
// 	}
// 	return (NULL);
// }
void	ft_setenv(char **env, char *key, char *value)
{
	int	nmemb;

	nmemb = 0;
	while (env[nmemb])
		nmemb++;
	env = ft_realloc(env, sizeof(char *) * nmemb + 1, sizeof(char *) * nmemb + 2);
	env[nmemb] = ezg_calloc(GLOBAL, sizeof(char), ft_strlen(key) + ft_strlen(value) + 2);
	ft_sprintf(env[nmemb], "%s=%s", key, value);
}

char	*ft_getenv(char **env, char *key)
{
	int		i;
	int		j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (key[j] && env[i][j] && key[j] == env[i][j])
			j++;
		if (!key[j] && env[i][j] == '=')
			return(ft_strchr(env[i], '=') + 1);
		i++;
	}
	return (NULL);
}

