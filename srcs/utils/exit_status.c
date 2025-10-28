#include "minishell.h"

typedef enum e_operation
{
	SET,
	GET
} t_operation;

static int	handle_exit_status(t_operation operation, int new_value)
{
	static int	return_value;

	if (operation == SET)
	{
		return_value = new_value;
	}
	return (return_value);
}

int		get_exit_status()
{
	return (handle_exit_status(GET, 0));
}

void	set_exit_status(int new_value)
{
	handle_exit_status(SET, new_value);
}
