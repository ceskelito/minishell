/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 12:35:23 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 12:19:46 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_ezg_groups(void)
{
	ezg_group_create(TOKEN);
	ezg_group_create(GLOBAL);
	ezg_group_create(COMMAND);
	ezg_group_create(EXECUTING);
	ezg_group_create(ENV);
}

void	init_shell(t_shell *shell)
{
	create_ezg_groups();
	shell->env_list = NULL;
	shell->cmd_list = NULL;
	shell->tokens = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	shell->interactive = 1;
	shell->std_out = dup(STDOUT_FILENO);
	shell->std_in = dup(STDIN_FILENO);
}
