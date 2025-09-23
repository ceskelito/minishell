/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:01:42 by rceschel          #+#    #+#             */
/*   Updated: 2025/03/04 10:46:45 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_lib.h"
#include <stdio.h>

static int	count_digits(long int n)
{
	int	d;

	if (n < 0)
		n *= -1;
	d = 1;
	n /= 10;
	while (n)
	{
		n /= 10;
		d++;
	}
	return (d);
}

char	*ft_itoa(int n)
{
	long int	nb;
	int			digits;
	char		*s;

	nb = n;
	digits = count_digits(n);
	if (n < 0)
	{
		nb *= -1;
		digits++;
	}
	s = ft_calloc(digits + 1, sizeof(char));
	if (!s)
		return (NULL);
	while (digits--)
	{
		s[digits] = nb % 10 + '0';
		nb /= 10;
	}
	if (n < 0)
		s[0] = '-';
	return (s);
}
/*
#include <stdio.h>

int	main(int ac, char **av)
{

	if(ac != 2)
	{
		printf("Numero argomenti errato!\n");
		return (1);
	}

	printf("%s\n", ft_itoa(atoi(av[1])));
	return (0);
}*/
