/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:06:17 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/19 10:34:02 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define SET 1
#define GET 2

static int	handle_exit_status(int operation, int new_value)
{
	static int	return_value;

	if (operation == SET)
	{
		return_value = new_value;
	}
	return (return_value);
}

int	get_exit_status(void)
{
	return (handle_exit_status(GET, 0));
}

void	set_exit_status(int new_value)
{
	handle_exit_status(SET, new_value);
}
