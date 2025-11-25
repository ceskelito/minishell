/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 12:34:56 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/25 12:35:03 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H
# define NOKEY 0
# define NOVALUE 0

enum
{
	GET,
	SET,
	UNSET,
	GET_ARRAY
};

char	**env_handler(int mode, char *key, char *value);
#endif
