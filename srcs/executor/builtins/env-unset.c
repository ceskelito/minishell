/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env-unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 16:23:22 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 16:27:06 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(char *const args[])
{
	int	i;

	i = 0;
	while (i++, args[i])
	{
		ft_unsetenv(args[i]);
	}
	set_exit_status(0);
}

void	env(void)
{
	char	**env_array;
	int		i;

	env_array = ft_getenv_array();
	i = 0;
	while (env_array[i])
	{
		ft_printf("%s\n", env_array[i]);
		i++;
	}
	set_exit_status(0);
}
