#include "minishell.h"

int print_error(char *msg, char *err)
{
    int len;

    len = ft_dprintf(STDERR_FILENO,
					"minishell: %s: %s\n", msg, err);
    return (len);
}