#include "minishell.h"
#include "ezgalloc.h"

int	handle_word_token(t_cmd *cmd, t_token **token)
{
	if (set_cmd_args(cmd, *token) < 0)
		return (-1);
	while ((*token)->next && ((*token)->next->type & WORD))
		*token = (*token)->next;
	return (0);
}

int	handle_redir_token(t_cmd *cmd, t_token **token)
{
	if (parse_redirection(cmd, token) != 0)
		return (-1);
	return (0);
}

int	handle_pipe_token(t_cmd **cmd, t_token *token)
{
	if (!token->next || !(token->next->type & WORD))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error: unexpected end of pipe\n");
		return (-1);
	}
	(*cmd)->pipe_output = true;
	add_pipe_redir(*cmd, OUT);
	if (go_next_cmd(cmd) < 0)
		return (-1);
	add_pipe_redir(*cmd, IN);
	return (0);
}
