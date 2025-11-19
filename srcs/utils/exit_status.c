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

typedef enum e_operation
{
	SET,
	GET
} t_operation;

static int	handle_exit_status(t_operation operation, int new_value)
{
	static int	return_value;

	if (operation == SET)
	{
		return_value = new_value;
	}
	return (return_value);
}

int		get_exit_status()
{
	return (handle_exit_status(GET, 0));
}

void	set_exit_status(int new_value)
{
	handle_exit_status(SET, new_value);
}
