#include "minishell.h"
#include "executor.h"
#include <unistd.h>

static void	add_pipe_redir(t_cmd *cmd, int fd, t_token_type type)
{
	t_redir	*redir;

	redir = ezg_calloc(GLOBAL, sizeof(t_redir), 1);
	if (!redir)
		return ;
	redir->file = NULL;
	redir->pipe_fd = fd;
	redir->type = PIPE | type;
	add_redir(cmd, redir);
}

int 	setup_pipe(t_cmd *cmd)
{
	int	fd[2];

	if (!cmd)
		return (1);
	if (!cmd->pipe_output)
		return (0);
	if (pipe(fd) == -1)
	{
		perror("minishell");
		return (-1);
	}
	add_pipe_redir(cmd, fd[1], OUT);
	add_pipe_redir(cmd->next, fd[0], IN);
	return (0);
}
