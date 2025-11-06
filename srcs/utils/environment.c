#include "minishell.h"
#include "environment.h"

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

	tmp = env_handler(GET, key, NOVALUE);
	if (!tmp)
		return (NULL);
	return (ft_strchr(*tmp, '=') + 1);
}

