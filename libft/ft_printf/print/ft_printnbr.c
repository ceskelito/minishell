/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ceskelito <ceskelito@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 16:26:11 by rceschel          #+#    #+#             */
/*   Updated: 2025/07/12 16:09:29 by ceskelito        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	count_digits(long int n)
{
	int	d;

	d = 1;
	if (n < 0)
	{
		n *= -1;
		d++;
	}
	n /= 10;
	while (n)
	{
		n /= 10;
		d++;
	}
	return (d);
}

int	ft_printnbr(int n)
{
	ft_putnbr_fd(n, 1);
	return (count_digits(n));
}
