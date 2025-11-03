#include "minishell.h"

typedef enum e_error_type
{
	ERR_SYSTEM,
	ERR_INTERNAL,
	ERR_SYNTAX
} t_error_type;

/*
 * handle_system_error - Handle system errors (like file not found, permission denied)
 * @context: Context where error occurred (e.g., "cd", "open")
 * @filename: File/command that caused error (can be NULL)
 */
void	handle_system_error(char *context, char *filename)
{
	char	*error_msg;

	if (filename)
	{
		error_msg = ezg_add(EXECUTING, ft_strjoin_three("minishell: ", context, ": "));
		error_msg = ezg_add(EXECUTING, ft_strjoin(error_msg, filename));
		perror(error_msg);
	}
	else
	{
		error_msg = ezg_add(EXECUTING, ft_strjoin("minishell: ", context));
		perror(error_msg);
	}
	
	// Set appropriate exit status based on errno
	if (errno == ENOENT)
		set_exit_status(127); // Command not found
	else if (errno == EACCES)
		set_exit_status(126); // Permission denied
	else
		set_exit_status(1);   // General error
}

/*
 * handle_internal_error - Handle internal errors (memory allocation, etc.)
 * @context: Context where error occurred
 * @details: Additional error details
 */
void	handle_internal_error(char *context, char *details)
{
	ft_dprintf(STDERR_FILENO, "minishell: internal error in %s", context);
	if (details)
		ft_dprintf(STDERR_FILENO, ": %s", details);
	ft_dprintf(STDERR_FILENO, "\n");
	
	set_exit_status(1);
}

/*
 * handle_syntax_error - Handle syntax errors
 * @token: Token that caused the error (can be NULL)
 * @message: Custom error message (can be NULL)
 */
void	handle_syntax_error(char *token, char *message)
{
	if (message)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s", message);
	}
	else if (token)
	{
		ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'", token);
	}
	else
	{
		ft_dprintf(STDERR_FILENO, "minishell: syntax error");
	}
	ft_dprintf(STDERR_FILENO, "\n");
	
	set_exit_status(2);
}

/*
 * handle_command_not_found - Handle command not found error
 * @command: Command that was not found
 */
void	handle_command_not_found(char *command)
{
	ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n", command);
	set_exit_status(127);
}

/*
 * handle_execution_error - Handle execution errors (fork, exec, etc.)
 * @command: Command that failed to execute
 * @error_type: Type of execution error
 */
void	handle_execution_error(char *command, t_error_type error_type)
{
	if (error_type == ERR_SYSTEM)
	{
		handle_system_error("exec", command);
	}
	else
	{
		handle_internal_error("execution", command);
	}
}

/*
 * handle_parsing_error - Handle parsing errors
 * @context: Parsing context (e.g., "tokenization", "command parsing")
 * @error_type: Type of parsing error
 */
void	handle_parsing_error(char *context, t_error_type error_type)
{
	if (error_type == ERR_SYSTEM)
	{
		handle_system_error("parsing", context);
	}
	else
	{
		handle_internal_error("parsing", context);
	}
}