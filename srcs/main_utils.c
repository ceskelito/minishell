/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:31:54 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/16 19:31:56 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_debug_command(char *input)
{
	return (ft_strncmp(input, "DEBUG:", 6) == 0);
}

static char	*extract_command(char *input)
{
	if (is_debug_command(input))
		return (input + 6);
	return (input);
}

static void	print_debug_info(t_shell *shell)
{
	printf("\n=== DEBUGGING INFO ===\n");
	print_tokens(shell->tokens);
	print_cmd_list(shell->cmd_list);
	printf("======================\n\n");
}

static void	handle_parse_error(t_shell *shell)
{
	set_exit_status(2);
	ezg_group_release(TOKEN);
	shell->tokens = NULL;
}

void	process_command(char *input, t_shell *shell)
{
	char	*cmd;
	int		debug_mode;

	debug_mode = is_debug_command(input);
	cmd = input;
	if (debug_mode)
		cmd = extract_command(input);
	shell->tokens = tokenize_input(cmd);
	if (!shell->tokens)
		return ((void)set_exit_status(2));
	concatenate_tokens(&(shell->tokens));
	shell->cmd_list = parse_tokens(shell->tokens);
	if (!shell->cmd_list)
		return (handle_parse_error(shell));
	if (debug_mode)
		print_debug_info(shell);
	executor(shell);
	ezg_group_release(TOKEN);
	ezg_group_release(COMMAND);
	shell->tokens = NULL;
	shell->cmd_list = NULL;
}
