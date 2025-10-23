#include "minishell.h"
#include "executor.h"

void	add_pipe_redir(t_cmd *cmd, int fd, t_token_type type)
{
	t_redir	*new;

	new = ezg_alloc(GLOBAL, sizeof(t_redir));
	new->file = NULL;
	new->pipe_fd = fd;
	new->type = PIPE | type;
	add_redir(cmd, new);
}

void 	setup_pipe(t_cmd *cmd)
{
	int	fd[2];
	int	parent;

	if (pipe(fd) == -1)
	{
		return ;
	}
	parent = fork();
	if (parent == -1)
	{
		return ;
	}
	if (parent)
	{
		add_pipe_redir(cmd->next, fd[1], IN);
	}
	else
	{
		add_pipe_redir(cmd, fd[0], OUT);
	}
}
