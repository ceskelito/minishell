/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:59:54 by abecucci          #+#    #+#             */
/*   Updated: 2024/12/10 11:50:14 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

unsigned int	ft_strlcat(char *dest, const char *src, unsigned int size)
{
	unsigned int	i;
	unsigned int	l;
	unsigned int	j;

	l = 0;
	while (dest[l] != '\0' && l < size)
		l++;
	j = 0;
	while (src[j] != '\0')
		j++;
	i = 0;
	if (size == 0 || l == size)
		return (l + j);
	while (i < size - l - 1 && src[i] != '\0')
	{
		dest[l + i] = src[i];
		i++;
	}
	dest[l + i] = '\0';
	return (l + j);
}
