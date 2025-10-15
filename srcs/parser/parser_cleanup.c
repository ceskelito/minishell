#include "minishell.h"

void	free_cmd_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_redirs(t_redir *redirs)
{
	t_redir	*temp;
	t_redir	*next;

	temp = redirs;
	while (temp)
	{
		next = temp->next;
		free(temp->file);
		free(temp);
		temp = next;
	}
}

void	free_single_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_cmd_args(cmd->args);
	free_redirs(cmd->redirs);
	free(cmd);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*temp;
	t_cmd	*next;

	temp = cmds;
	while (temp)
	{
		next = temp->next;
		free_single_cmd(temp);
		temp = next;
	}
}

void	cleanup_parsing(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->cmd_list)
	{
		free_cmds(shell->cmd_list);
		shell->cmd_list = NULL;
	}
}