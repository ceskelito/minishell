#include "minishell.h"

void	init_shell(t_shell *shell)
{
	shell->env_list = NULL;
	shell->cmd_list = NULL;
	shell->tokens = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	shell->interactive = 1;
	shell->std_out = dup(STDOUT_FILENO);
	shell->std_in = dup(STDIN_FILENO);
}
