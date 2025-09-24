#include "minishell.h"

/* Global variable for signal handling */
extern int	g_sig_status;

/**
 * Initializes the shell structure
 * Sets up environment variables and other initial settings
 */
void	init_shell(t_shell *shell, char **envp)
{
	shell->env_list = init_env(envp);
	shell->env_array = NULL;
	shell->cmd_list = NULL;
	shell->tokens = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	shell->interactive = isatty(STDIN_FILENO);
	env_to_array(shell);
}

/**
 * Processes a single command line
 * Returns 1 if shell should continue, 0 if it should exit
 */
int	process_line(t_shell *shell)
{
	int	result;

	result = 1;
	if (shell->line && *shell->line)
	{
		add_history(shell->line);
		shell->tokens = tokenize_input(shell->line);
		if (shell->tokens && check_syntax(shell->tokens) == SUCCESS)
		{
			expand_tokens(shell->tokens, shell);
			shell->cmd_list = parse_tokens(shell->tokens, shell);
			if (shell->cmd_list)
				shell->exit_status = execute_cmds(shell->cmd_list, shell);
			if (shell->exit_status == -1)
				result = 0;
		}
		free_tokens(shell->tokens);
		free_cmds(shell->cmd_list);
		shell->tokens = NULL;
		shell->cmd_list = NULL;
	}
	free(shell->line);
	shell->line = NULL;
	return (result);
}

/**
 * Main shell loop
 * Reads lines from user, processes them until exit
 */
void	shell_loop(t_shell *shell)
{
	char	*prompt;

	while (1)
	{
		setup_signals(1);
		prompt = ft_strdup("minishell> ");
		if (!prompt)
			ft_error("Memory allocation error", 1);
		shell->line = readline(prompt);
		free(prompt);
		if (!shell->line)
		{
			if (shell->interactive)
				ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (!process_line(shell))
			break ;
	}
}

/**
 * Main function
 * Entry point of the shell
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	g_sig_status = 0;
	init_shell(&shell, envp);
	shell_loop(&shell);
	free_shell(&shell);
	rl_clear_history();
	return (shell.exit_status);
}
