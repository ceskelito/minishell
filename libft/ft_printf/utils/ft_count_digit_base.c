/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_digit_base.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:54:54 by rceschel          #+#    #+#             */
/*   Updated: 2025/01/04 16:30:23 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	count_digit_base(long nbr, int base_size)
{
	int	i;

	i = 0;
	if (nbr == 0)
		return (1);
	if (nbr < 0)
	{
		nbr *= -1;
		i++;
	}
	while (nbr > 0)
	{
		nbr /= base_size;
		i++;
	}
	return (i);
}
