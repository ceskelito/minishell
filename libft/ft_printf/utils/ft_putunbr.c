/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:51:55 by rceschel          #+#    #+#             */
/*   Updated: 2025/01/03 17:03:16 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putunbr(unsigned int n)
{
	char	digit;

	if (n < 10)
	{
		digit = n + '0';
		write(1, &digit, 1);
		return ;
	}
	digit = (n % 10) + '0';
	ft_putunbr(n / 10);
	write(1, &digit, 1);
}
