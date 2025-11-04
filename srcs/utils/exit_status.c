#include "minishell.h"

static int g_exit_status = 0;

int	get_exit_status(void)
{
	return (g_exit_status);
}

void	set_exit_status(int new_value)
{
	g_exit_status = new_value;
	g_sig_status = new_value;  // Синхронизируем с глобальной переменной
}