/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:16:30 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/20 16:14:35 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

void	close_pipe_fds(t_cmd *cmd, int redir_type, int cmd_to_parse)
{
	t_cmd	*curr_cmd;
	t_redir	*curr_redir;
	int		num_of_parsed;

	num_of_parsed = 0;
	curr_cmd = cmd;
	while (curr_cmd && (cmd_to_parse == -1 || num_of_parsed < cmd_to_parse))
	{
		curr_redir = curr_cmd->redirs;
		while (curr_redir)
		{
			if (curr_redir->type & (redir_type))
				close(curr_redir->pipe_fd);
			curr_redir = curr_redir->next;
		}
		curr_cmd = curr_cmd->next;
		num_of_parsed++;
	}
}

static int	open_redir(t_redir *r)
{
	if (r->type & (PIPE | HEREDOC))
		return (r->pipe_fd);
	if (r->type & APPEND)
		return (open(r->file, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC,
				0644));
	if (r->type & OUT)
		return (open(r->file, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC,
				0644));
	if (r->type & IN)
		return (open(r->file, O_RDONLY | O_CLOEXEC));
	return (-1);
}

int	apply_redirs(t_redir *r)
{
	int		fd;

	while (r)
	{
		fd = open_redir(r);
		if (fd == -1)
			return (print_error(r->file, strerror(errno)), errno);
		if (r->type & IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		r = r->next;
	}
	return (0);
}
