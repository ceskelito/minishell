#include "minishell.h"

int print_error(char *msg, char *err)
{
    int len;

    len = ft_dprintf(STDERR_FILENO,
					"minishell: %s: %s\n", msg, err);
    return (len);
}

bool	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset"));
}
