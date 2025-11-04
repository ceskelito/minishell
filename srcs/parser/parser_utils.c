#include "minishell.h"

/**
 * Checks if token is a redirection token
 */
int	is_redir_token(t_token_type type)
{
	return (type == REDIR_IN_TOKEN || type == REDIR_OUT_TOKEN ||
			type == REDIR_APPEND_TOKEN || type == HEREDOC_TOKEN);
}

/**
 * Parses a redirection and adds it to command
 */
int	parse_redirection(t_cmd *cmd, t_token **token)
{
	t_redir_type	type;
	t_redir			*redir;

	if (!*token || !(*token)->next)
		return (0);
	type = get_redir_type((*token)->type);
	*token = (*token)->next;
	if ((*token)->type != WORD)
		return (0);
	redir = create_redir(type, (*token)->value);
	if (!redir)
		return (0);
	add_redir(cmd, redir);
	return (1);
}

/**
 * Counts arguments in current command
 */
int	count_args(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
}

/**
 * Adds an argument to command's argument list
 */
int	add_arg(t_cmd *cmd, char *arg)
{
	int		count;
	char	**new_args;
	int		i;

	count = count_args(cmd->args);
	new_args = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
	{
		free(new_args);
		return (0);
	}
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (1);
}