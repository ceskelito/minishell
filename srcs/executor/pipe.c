#include "minishell.h"
#include "executor.h"
#include <unistd.h>

static void	add_pipe_redir(t_cmd *cmd, int fd, t_token_type type)
{
	t_redir	*new;

	new = ezg_calloc(GLOBAL, sizeof(t_redir), 1);
	if (!new)
		return ;
	new->file = NULL;
	new->pipe_fd = fd;
	new->type = PIPE | type;
	add_redir(cmd, new);
}

bool 	setup_pipe(t_cmd *cmd)
{
	int	fd[2];

	if (!cmd)
		return (false); // bash: syntax error near unexpected token `|'
	if (!cmd->pipe_output)
		return (true);
	// cmd->next = NULL;
	if (!cmd->next)
	{
		ft_dprintf(STDERR_FILENO, "minishell: syntax error: unclosed pipe\n");
		return (false);
	}
	if (pipe(fd) == -1)
		return (false);
	add_pipe_redir(cmd, fd[1], OUT);
	add_pipe_redir(cmd->next, fd[0], IN);
	return (true);
}
