#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"
#include <unistd.h>

int	is_redir_token(t_token_type type)
{
	return (type & (IN | OUT | APPEND | HEREDOC));
}

bool is_redirection_valid(t_token **token)
{
	if (!*token)
		return (false);
	if (!(*token)->next)
	{
		ft_dprintf(STDERR_FILENO,"minishell: syntax error near unexpected token `%s'",
			"newline");
		return (false);
	}
	if ((*token)->next->type != WORD)
	{
		ft_dprintf(STDERR_FILENO,"minishell: syntax error near unexpected token `%s'",
			(*token)->value);
		return (false);
	}
	return (true);
}

t_redir	*create_redir(int type, char *file)
{
	t_redir	*redir;

	redir = ezg_alloc(COMMAND, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ezg_add(COMMAND, ft_strdup(file));
	if (!redir->file)
		return (NULL);
	redir->next = NULL;
	return (redir);
}

void	add_redir(t_cmd *cmd, t_redir *redir)
{
	t_redir	*temp;

	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		temp = cmd->redirs;
		while (temp->next)
			temp = temp->next;
		temp->next = redir;
	}
}

int	parse_redirection(t_cmd *cmd, t_token **token)
{
	int		redir_type;
	t_redir	*redir;

	if (!is_redirection_valid(token))
		return (-1);
	redir_type = (*token)->type;
	redir = create_redir(redir_type, (*token)->next->value);
	if (!redir)
		return (-1);
	add_redir(cmd, redir);
	*token = (*token)->next;
	return (0);
}
