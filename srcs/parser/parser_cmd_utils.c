#include "minishell.h"
#include "ezgalloc.h"

int	token_count_args(t_token *token)
{
	int		count;

	count = 0;
	while (token && (token->type & WORD))
	{
		count++;
		token = token->next;
	}
	return (count);
}

int	set_cmd_args(t_cmd *cmd, t_token *token)
{
	int		i;
	int		count;

	count = token_count_args(token);
	cmd->args = ezg_calloc(COMMAND, count + 1, sizeof(char *));
	if (!cmd->args)
		return (-1);
	i = 0;
	while (i < count)
	{
		cmd->args[i] = ft_strdup(token->value);
		if (!cmd->args[i])
			return (-1);
		ezg_add(COMMAND, cmd->args[i]);
		token = token->next;
		i++;
	}
	cmd->args[i] = NULL;
	return (count);
}

int	go_next_cmd(t_cmd **cmd)
{
	(*cmd)->next = ezg_calloc(COMMAND, 1, sizeof(t_cmd));
	if (!(*cmd)->next)
		return (-1);
	*cmd = (*cmd)->next;
	return (0);
}

void	add_pipe_redir(t_cmd *cmd, t_token_type type)
{
	t_redir	*r;

	r = ezg_calloc(GLOBAL, 1, sizeof(t_redir));
	if (!r)
		return ;
	r->type = PIPE | type;
	r->pipe_fd = -1;
	r->file = NULL;
	add_redir(cmd, r);
}