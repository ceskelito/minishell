/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:39:42 by ceskelito         #+#    #+#             */
/*   Updated: 2025/11/07 18:33:07 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "environment.h"

char	**ft_getenv_array()
{
	return (env_handler(GET_ARRAY, NOKEY, NOVALUE));
}

int		ft_setenv(char *key, char *value)
{
	if (!key)
		return (-1);
	if (!value)
		value = "";
	env_handler(SET, key, value);
	return (0);
}

char	*ft_getenv(char *key)
{
	char	**tmp;

	tmp = env_handler(GET, key, NOVALUE);
	if (!tmp)
		return (NULL);
	return (ft_strchr(*tmp, '=') + 1);
}

void	ft_unsetenv(char *key)
{
	if (!key)
		return ;
	env_handler(UNSET, key, NOVALUE);
}

