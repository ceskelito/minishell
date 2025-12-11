/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 10:10:43 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 12:22:27 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>

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

static void	process_command(char *input, t_shell *shell)
{
	char	*cmd;
	int		debug_mode;

	debug_mode = is_debug_command(input);
	if (debug_mode)
		cmd = extract_command(input);
	else
		cmd = input;
	shell->tokens = tokenize_input(cmd);
	concatenate_tokens(&(shell->tokens));
	shell->cmd_list = parse_tokens(shell->tokens);
	if (debug_mode)
		print_debug_info(shell);
	executor(shell);
}

void	set_signal(int signum, void (*handler)(int));	//signals.c
void	handle_sigint(int signal);						// signals.c
void	readline_on_buff(char **buff);					// get_prompt.c

int	main(void)
{
	t_shell	shell;
	char	*input;
	char	*cwd;

	init_shell(&shell);
	set_signal(SIGINT, handle_sigint);
	set_signal(SIGQUIT, SIG_IGN);
	printf("Welcome to minishell!\n");
	printf("Type 'DEBUG: command' to see tokenization and parsing.\n\n");
	while (readline_on_buff(&input), input)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			perror("minishell");
			set_exit_status(errno);
			break ;
		}
		if (ft_strcmp(cwd, ft_getenv("PWD")) != 0)
			ft_setenv("PWD", cwd);
		free(cwd);
		add_history(input);
		process_command(input, &shell);
	}
	exit_shell(NULL);
}
