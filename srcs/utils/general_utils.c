/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:06:21 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/17 20:06:22 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int print_error(char *msg, char *err)
{
    int len;

    len = ft_dprintf(STDERR_FILENO,
					"minishell: %s: %s\n", msg, err);
    return (len);
}