/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:23:39 by rceschel          #+#    #+#             */
/*   Updated: 2025/03/04 10:46:45 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_lib.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	uc;
	unsigned char	un;
	unsigned char	*buffer;
	int				i;

	buffer = (unsigned char *)s;
	uc = (unsigned char)c;
	un = (unsigned char)n;
	i = 0;
	while (i < un)
	{
		if (buffer[i] == uc)
			return ((void *)s + i);
		i++;
	}
	return (NULL);
}
