#include "minishell.h"
#include <unistd.h>

void	init_shell(t_shell *shell, char **envp)
{
	shell->env_list = NULL;
	shell->cmd_list = NULL;
	shell->tokens = NULL;
	shell->env_array = envp;
	shell->line = NULL;
	shell->exit_status = 0;
	shell->interactive = 1;
	shell->std_out = dup(STDOUT_FILENO);
	shell->std_in = dup(STDIN_FILENO);
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->cmd_list)
		free_cmds(shell->cmd_list);
	if (shell->line)
		free(shell->line);
	shell->env_list = NULL;
	shell->tokens = NULL;
	shell->cmd_list = NULL;
	shell->line = NULL;
}