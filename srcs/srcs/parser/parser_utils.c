#include "ezgalloc.h"
#include "minishell.h"

int	is_redir_token(t_token_type type)
{
	//return (type & IN || type & OUT || type & APPEND || type & HEREDOC);
	return (type & (IN | OUT | APPEND | HEREDOC));
}

int	parse_redirection(t_cmd *cmd, t_token **token)
{
	int		type;
	t_redir	*redir;

	if (!*token || !(*token)->next)
		return (0);
	type = (*token)->type;
	*token = (*token)->next;
	if ((*token)->type != WORD)
		return (0);
	redir = create_redir(type, (*token)->value);
	if (!redir)
		return (0);
	add_redir(cmd, redir);
	return (1);
}

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

//static char	**allocate_new_args(int count)
//{
//	char	**new_args;
//
//	new_args = ezg_alloc(GLOBAL, sizeof(char *) * (count + 2));
//	return (new_args);
//}

int	add_arg(t_cmd *cmd, char *arg)
{
	int		count;
	char	**new_args;
	int		i;

	count = count_args(cmd->args);
	new_args = ezg_alloc(GLOBAL, sizeof(char *) * (count + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ezg_add(GLOBAL, ft_strdup(arg));
	if (!new_args[i])
	{
		//free(new_args);
		return (0);
	}
	new_args[i + 1] = NULL;
	ezg_release(GLOBAL, cmd->args);
	cmd->args = new_args;
	return (1);
}
