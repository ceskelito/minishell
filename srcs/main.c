/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 10:10:43 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/12 16:35:28 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_signal(int signum, void (*handler)(int));
void	handle_sigint(int signal);
void	readline_on_buff(char **buff);
void	process_command(char *input, t_shell *shell);

static void	update_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell");
		set_exit_status(1);
		exit_shell(NULL);
	}
	if (ft_strcmp(cwd, ft_getenv("PWD")) != 0)
		ft_setenv("PWD", cwd);
	free(cwd);
}

int	main(void)
{
	t_shell	shell;
	char	*input;

	init_shell(&shell);
	set_signal(SIGINT, handle_sigint);
	set_signal(SIGQUIT, SIG_IGN);
	printf("Welcome to minishell!\n");
	printf("Type 'DEBUG: command' to see tokenization and parsing.\n\n");
	while (readline_on_buff(&input), input)
	{
		update_pwd();
		add_history(input);
		process_command(input, &shell);
	}
	exit_shell(NULL);
}
