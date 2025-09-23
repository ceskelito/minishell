/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_multi.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:40:15 by rceschel          #+#    #+#             */
/*   Updated: 2025/03/12 15:45:21 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_lib.h"
#include <stdarg.h>
#include <string.h>

char	*ft_strjoin_multi(int num, ...)
{
	char	*final_string;
	char	*ptr;
	va_list	args;

	va_start(args, num);
	final_string = ft_strdup("");
	while (num > 0)
	{
		ptr = final_string;
		final_string = ft_strjoin(final_string, va_arg(args, const char *));
		free(ptr);
		if (!final_string)
			return (NULL);
		num--;
	}
	return (final_string);
}
