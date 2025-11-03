#include "executor.h"
#include "ezgalloc.h"
#include "minishell.h"

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
	
	// Простая токенизация без дополнительных проверок
	shell->tokens = tokenize_input(cmd);
	if (!shell->tokens)
	{
		printf("Error: tokenization failed\n");
		return;
	}
	
	// Простой парсинг
	shell->cmd_list = parse_tokens(shell->tokens);
	if (!shell->cmd_list)
	{
		printf("Error: parsing failed\n");
		return;
	}
	
	// Debug или выполнение
	if (debug_mode)
		print_debug_info(shell);
	else
		executor(shell);
}

void __attribute__((destructor)) ezg_cleanup();
void __attribute__((constructor)) create_groups();

void	create_groups(void)
{
	ezg_group_create(TOKEN);
	ezg_group_create(GLOBAL);
	ezg_group_create(COMMAND);
	ezg_group_create(EXECUTING);
}

static char	*get_prompt(void)
{
	char	*working_directory;
	char	*user;
	char	*prompt;
	size_t	prompt_len;

	// Используем простые функции вместо wrapper'ов
	user = getenv("USER");
	if (!user)
		user = "user";
		
	working_directory = getcwd(NULL, 0);
	if (!working_directory)
		working_directory = ft_strdup("unknown");
	
	prompt_len = ft_strlen(user) + ft_strlen(working_directory) + 
				 (ft_strlen(GREEN) * 4) + 3 + 1;
	prompt = ezg_alloc(EXECUTING, sizeof(char) * prompt_len);
	if (!prompt)
	{
		free(working_directory);
		return (ezg_add(EXECUTING, ft_strdup("$ ")));
	}
	
	ft_sprintf(prompt, "%s%s%s%c%s%s%s%c%c",
				GREEN, user, 
				DEFAULT, ':',
				BLUE, working_directory,
				DEFAULT, '$', ' ');
	free(working_directory);
	return (prompt);
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
		input = ezg_add(EXECUTING, readline(get_prompt()));
		if (!input)
			break;
		
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			process_command(input, &shell);
		}
		
		ezg_group_release(EXECUTING);
	}
	
	printf("\nGoodbye!\n");
	return (get_exit_status());
}