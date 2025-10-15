#include "ft_lib.h"
#include "ft_sprintf.h"
#include "minishell.h"
#include <unistd.h>

#define GREEN "\33[32m"
#define BLUE "\33[34m"
#define DEFAULT "\33[39m"

int	g_sig_status = 0;

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
	cmd = extract_command(input);
	shell->tokens = tokenize_input(cmd);
	shell->cmd_list = parse_tokens(shell->tokens, shell);
	if (debug_mode)
		print_debug_info(shell);
	else
	{
		/* TODO: executor будет здесь */
		printf("Command ready for execution\n");
	}
	cleanup_parsing(shell);
}

int	main(void)
{
	extern char	**environ;
	t_shell		shell;
	char		*input;

	init_shell(&shell, environ);
	printf("Welcome to minishell!\n");
	printf("Type 'DEBUG: command' to see tokenization and parsing.\n\n");
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			process_command(input, &shell);
		}
		free(input);
	}
	cleanup_shell(&shell);
	printf("\nGoodbye!\n");
	return (0);
}