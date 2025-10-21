#include "minishell.h"

char	*ft_strjoin_char(char *s1, char c)
{
	char	*result;
	int		len;
	int		i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	result = malloc(sizeof(char) * (len + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	result[i] = c;
	result[i + 1] = '\0';
	return (result);
}
