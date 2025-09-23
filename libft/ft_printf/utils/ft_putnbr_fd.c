/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:51:55 by rceschel          #+#    #+#             */
/*   Updated: 2024/12/27 11:12:28 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdbool.h>
#include <unistd.h>

bool	is_min(int n, int fd)
{
	if (n == INT_MIN)
	{
		write(fd, "-2147483648", 11);
		return (true);
	}
	return (false);
}

void	ft_putnbr_fd(int n, int fd)
{
	char	digit;

	if (is_min(n, fd))
		return ;
	if (n < 0)
	{
		n *= -1;
		write(fd, "-", 1);
	}
	if (n < 10)
	{
		digit = n + '0';
		write(fd, &digit, 1);
		return ;
	}
	digit = (n % 10) + '0';
	ft_putnbr_fd(n / 10, fd);
	write(fd, &digit, 1);
}
