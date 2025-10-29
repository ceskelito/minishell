#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"
#include <unistd.h>

int	is_redir_token(t_token_type type)
{
	//return (type & IN || type & OUT || type & APPEND || type & HEREDOC);
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
	return (0);
}

/* NOT USED 
int	cmd_count_args(char **args)
{
	int	count;

	count = 0;
	if (!args)
		return (0);
	while (args[count])
		count++;
	return (count);
} */

//static char	**allocate_new_args(int count)
//{
//	char	**new_args;
//
//	new_args = ezg_alloc(GLOBAL, sizeof(char *) * (count + 2));
//	return (new_args);
//}

/* NOT USED
int	add_arg(char **args, char *arg)
{
	int		count;
	char	**new_args;
	int		i;

	count = cmd_count_args(args);
	new_args = ezg_alloc(COMMAND, sizeof(char *) * (count + 2));
	if (!new_args)
		return (0);
	i = 0;
	while (i < count)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i] = ezg_add(GLOBAL, ft_strdup(arg));
	if (!new_args[i])
	{
		//free(new_args);
		return (0);
	}
	new_args[i + 1] = NULL;
	ezg_release(COMMAND, args);
	args = new_args;
	return (1);
} */
