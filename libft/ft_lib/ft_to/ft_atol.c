/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 11:28:28 by rceschel          #+#    #+#             */
/*   Updated: 2025/03/12 15:46:10 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	is_space(char c)
{
	if (c == ' ' || (c == '\f') || (c == '\n'))
		return (1);
	else if (c == '\r' || (c == '\t') || (c == '\v'))
		return (1);
	return (0);
}

long int	ft_atol(const char *str)
{
	long int	sign;
	int			i;
	long int	nb;

	while (is_space(*str))
		str++;
	nb = 0;
	sign = 1;
	if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	i = 0;
	while (str[i] >= '0' && (str[i] <= '9'))
	{
		if (i != 0)
			nb *= 10;
		nb += (str[i++] - '0');
	}
	return (nb * sign);
}
