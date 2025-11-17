/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:27 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/17 19:50:13 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	clean_char(char **c)
{
	free(*c);
}

void	clean_array(char ***s)
{
	int		i;
	char	**array;

	array = *s;
	i = 0;
	while (array && array[i])
	{
		if (array[i]) 
			free(array[i]);
		i++;
	}
	if (array)
		free(array);
}
