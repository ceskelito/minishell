#include "executor.h"
#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"
#include <signal.h>
#include <unistd.h>

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
	if (debug_mode)
		cmd = extract_command(input); // cmd = input -- extract is only for debug ppourposes
	else
		cmd = input;
	shell->tokens = tokenize_input(cmd);
	concatenate_tokens(&(shell->tokens));
	shell->cmd_list = parse_tokens(shell->tokens);
	if (debug_mode)
		print_debug_info(shell);
	executor(shell);
}

void __attribute__((destructor)) ezg_cleanup();
void __attribute__((constructor)) create_groups();

void	create_groups()
{
	ezg_group_create(TOKEN);
	ezg_group_create(GLOBAL);
	ezg_group_create(COMMAND);
	ezg_group_create(EXECUTING);
	ezg_group_create(ENV);
}

static char	*prompt_getcwd(char *home_symbol)
{
	char	*home_path;
	char	*new_directory;
	char	*directory;

	directory = getcwd(NULL, 0);
	if (!directory)
		return (perror("minishell"), NULL);
	home_path = ft_getenv("HOME");
	if (!home_path)
		return (directory);
	if (ft_strnstr(directory, home_path, ft_strlen(home_path)))
	{
		/* allocate new string for prompt (home_symbol + rest of path) */
		new_directory = ft_calloc(ft_strlen(home_symbol) +
								ft_strlen(directory + ft_strlen(home_path)) +
								1,
								sizeof(char));
		if (!new_directory)
			return (directory);
		ft_sprintf(new_directory, "%s%s", home_symbol, directory + ft_strlen(home_path));
		free(directory);
		return (new_directory);
	}
	return (directory);
}

static char    *get_prompt()
{
    char    *working_directory;
    char    *user;
    char    *prompt;
    size_t  prompt_len;

    user = ft_getenv("USER");
	working_directory = prompt_getcwd(HOME_SYMBOL);
	if (!user)
		user = "\0";
	if (!working_directory)
		working_directory = "\0";
    prompt_len = ft_strlen(user) +
                 ft_strlen(working_directory) + 
                 (ft_strlen(GREEN) * 4) + 
                 3 + 1;
    prompt = malloc(sizeof(char) * prompt_len);
    ft_sprintf(prompt, "%s%s%s%c%s%s%s%c%c",
                GREEN, user, 
                DEFAULT, ':',
                BLUE, working_directory,
                DEFAULT, '$', ' ');
    free(working_directory);
    return (ezg_add(EXECUTING, prompt));
}

void	handle_sigint(int signal)
{
	//extern unsigned long	rl_readline_state;
	(void)signal;
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_sig_status = SIGINT;
	}
}

void set_signal(int signum, void (*handler)(int)) {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    sigaction(signum, &sa, NULL);
}

int	main(void)
{
	t_shell				shell;
	char				*input;
	char				*cwd;

	set_signal(SIGINT, handle_sigint);
	set_signal(SIGQUIT, SIG_IGN);
	init_shell(&shell);
	printf("Welcome to minishell!\n");
	printf("Type 'DEBUG: command' to see tokenization and parsing.\n\n");
	
	while (1)
	{
		input = ezg_add(EXECUTING, readline(get_prompt()));
		if (!input)
			break ;
		cwd = getcwd(NULL, 0);
		if (!cwd)
			return (perror("minishell"), errno);
		if (ft_strcmp(cwd, ft_getenv("PWD")) != 0)
			ft_setenv("PWD", cwd);
		free(cwd);
		add_history(input);
		process_command(input, &shell);
	}
	return (0);
}
