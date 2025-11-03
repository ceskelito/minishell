#include "minishell.h"

typedef enum e_operation
{
	SET,
	GET
} t_operation;

/*
 * handle_exit_status - Internal handler for exit status operations
 * @operation: SET or GET operation
 * @new_value: New value to set (ignored for GET)
 * Return: Current exit status value
 */
static int	handle_exit_status(t_operation operation, int new_value)
{
	static int	return_value = 0;

	if (operation == SET)
	{
		return_value = new_value;
		g_sig_status = new_value; // ✅ Синхронизируем с глобальной переменной
	}
	return (return_value);
}

/*
 * get_exit_status - Get current exit status
 * Return: Current exit status value
 */
int	get_exit_status(void)
{
	return (handle_exit_status(GET, 0));
}

/*
 * set_exit_status - Set new exit status value
 * @new_value: New exit status to set
 */
void	set_exit_status(int new_value)
{
	handle_exit_status(SET, new_value);
}
