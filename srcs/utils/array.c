/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 18:26:13 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/06 12:42:25 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** CHANGE DESRIPTION !!!
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
char	**dup_array(char *group, char **srcs)
{
	int		i;
	int		nmemb;
	char	**env;

	nmemb = 0;
	while (srcs[nmemb])
		nmemb++;
	env = ezg_calloc(group, sizeof(char *), nmemb + 1);
	i = 0;
	while (srcs[i])
	{
		env[i] = ft_strdup(srcs[i]);
		ezg_add(group, env[i]);
		i++;
	}
    env[i] = NULL;
	return (env);
}

/**
 * expand_array - Expand or shrink a dynamically allocated NULL-terminated array of strings.
 *
 * This function reallocates the given array by the specified increment.
 * If @increment is positive, the array is expanded; if negative, it is reduced.
 * If the resulting size is zero or negative, the entire array and its contents are released.
 *
 * @group      Identifier of the ezgalloc memory group used for allocation.
 * @array      Pointer to the NULL-terminated array to be modified.
 * @nmemb      Number of elements currently stored in the array.
 * @increment  Number of elements to add (positive) or remove (negative).
 *
 * Return: A pointer to the new array if successful, or NULL if the array
 *         was released or allocation failed.
 */
char	**expand_array(char *group, char **array, int nmemb, int increment)
{
	char	**new;

	if (!array)
		return (NULL);
	if (increment == 0)
		return (array);
	if ((nmemb + increment) <= 0)
	{
		while (nmemb-- > 0)
			ezg_release(group, array[nmemb]);
		ezg_release(group, array);
		return (NULL);
	}
	new = ezg_calloc(group, sizeof(char *), nmemb + increment + 1);
	if (!new)
		return (array);
	ft_memcpy(new, array, nmemb * sizeof(char *));
	ezg_release(group, array);
	return (new);
}