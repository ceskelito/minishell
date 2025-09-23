/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:10:55 by rceschel          #+#    #+#             */
/*   Updated: 2025/03/04 10:46:45 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_lib.h"
#include <stdbool.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*buff_src;
	unsigned char		*buff_dest;
	bool				to_left;
	size_t				i;

	if (!src && !dest)
		return (NULL);
	buff_src = src;
	buff_dest = dest;
	to_left = (buff_dest < buff_src);
	i = 0;
	while (i < n)
	{
		if (to_left)
			buff_dest[i] = buff_src[i];
		else
			buff_dest[n - i - 1] = buff_src[n - i - 1];
		i++;
	}
	return (dest);
}
