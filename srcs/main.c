/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 10:10:43 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 16:26:20 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




static void	process_command(char *input, t_shell *shell)
{
	char	*cmd;



	cmd = input;
	shell->tokens = tokenize_input(cmd);
	concatenate_tokens(&(shell->tokens));
	shell->cmd_list = parse_tokens(shell->tokens);

	executor(shell);
	ezg_group_release(TOKEN);
	ezg_group_release(COMMAND);
	shell->tokens = NULL;
	shell->cmd_list = NULL;
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
