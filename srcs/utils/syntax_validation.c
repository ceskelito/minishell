#include "minishell.h"

/*
 * check_unclosed_quotes - Check for unclosed quotes in input
 * @input: Input string to check
 * Return: 0 if valid, -1 if unclosed quotes found
 */
int	check_unclosed_quotes(char *input)
{
	int		i;
	char	quote_char;
	bool	in_quote;

	i = 0;
	in_quote = false;
	quote_char = 0;
	
	while (input[i])
	{
		if (!in_quote && (input[i] == '\'' || input[i] == '"'))
		{
			in_quote = true;
			quote_char = input[i];
		}
		else if (in_quote && input[i] == quote_char)
		{
			in_quote = false;
			quote_char = 0;
		}
		i++;
	}
	
	if (in_quote)
	{
		ft_dprintf(STDERR_FILENO, "minishell: syntax error: unclosed quote\n");
		set_exit_status(2);
		return (-1);
	}
	
	return (0);
}

/*
 * check_pipe_syntax - Check for pipe syntax errors
 * @tokens: Token list to check
 * Return: 0 if valid, -1 if syntax error
 */
int	check_pipe_syntax(t_token *tokens)
{
	t_token	*curr;
	bool	expect_command;

	curr = tokens;
	expect_command = true;
	
	while (curr)
	{
		if (curr->type & PIPE)
		{
			if (expect_command)
			{
				ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `|'\n");
				set_exit_status(2);
				return (-1);
			}
			expect_command = true;
		}
		else if (curr->type == WORD)
		{
			expect_command = false;
		}
		curr = curr->next;
	}
	
	if (expect_command)
	{
		ft_dprintf(STDERR_FILENO, "minishell: syntax error: unexpected end of file\n");
		set_exit_status(2);
		return (-1);
	}
	
	return (0);
}

/*
 * check_redir_syntax - Check redirection syntax
 * @tokens: Token list to check
 * Return: 0 if valid, -1 if syntax error
 */
int	check_redir_syntax(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (is_redir_token(curr->type))
		{
			if (!curr->next)
			{
				ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
				set_exit_status(2);
				return (-1);
			}
			if (curr->next->type != WORD)
			{
				ft_dprintf(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'\n", 
					curr->next->value);
				set_exit_status(2);
				return (-1);
			}
		}
		curr = curr->next;
	}
	
	return (0);
}

/*
 * validate_syntax - Comprehensive syntax validation
 * @input: Original input string
 * @tokens: Parsed tokens
 * Return: 0 if valid, -1 if syntax error
 */
int	validate_syntax(char *input, t_token *tokens)
{
	if (check_unclosed_quotes(input) == -1)
		return (-1);
	
	if (!tokens)
		return (0);
	
	if (check_pipe_syntax(tokens) == -1)
		return (-1);
	
	if (check_redir_syntax(tokens) == -1)
		return (-1);
	
	return (0);
}