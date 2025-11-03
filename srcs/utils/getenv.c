#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"

#define GET 1
#define SET 2

void	expand_array(char ***array, int *count)
{
	int		i;
	char	**new;

	new = ezg_calloc(GLOBAL, sizeof(char *), *count + 1);
	i = 0;
	while (i < *count)
	{
		new[i] = (*array)[i];
		i++;
	}
	new[i + 1] = NULL;
	new[i + 2] = NULL;
	ezg_release(GLOBAL, (*array));
	(*array) = new;
	(*count) += 1;
	new = NULL;
}

char	*env_handler(int mode, char *key, char *value)
{
	static char	**env;
	static int	count;

	if (mode == SET)
	{
		expand_array(&env, &count);
		env[count - 1] = ezg_calloc(GLOBAL, sizeof(char),
		                    ft_strlen(key) + ft_strlen(value) + 2);
		ft_sprintf(env[count], "%s=%s\0", key, value);
	}
	else if (mode == GET)	
	{
		// poi
	}
	return (NULL);
}
void	ft_setenv(char *key, char *value)
{

	if (ft_strcmp(value, "===") == 0)
		return ()
}

char	*ft_getenv(char *key)
{
	
}

