#include "minishell.h"

static void	print_redir_type(int type)
{
	if (type & IN)
		printf("<");
	else if (type & OUT)
		printf(">");
	else if (type & APPEND)
		printf(">>");
	else if (type & HEREDOC)
		printf("<<");
}

static void	print_cmd_args(char **args)
{
	int	i;

	if (!args)
		return ;
	printf("    Args: ");
	i = 0;
	while (args[i])
	{
		printf("'%s'", args[i]);
		if (args[i + 1])
			printf(", ");
		i++;
	}
	printf("\n");
}

static void	print_cmd_redirs(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		printf("    Redir: ");
		print_redir_type(current->type);
		printf(" %s\n", current->file);
		current = current->next;
	}
}

void	print_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	int		cmd_num;

	printf("AST (Command List):\n");
	if (!cmd_list)
	{
		printf("  (empty)\n");
		return ;
	}
	current = cmd_list;
	cmd_num = 1;
	while (current)
	{
		printf("  Command %d:\n", cmd_num);
		print_cmd_args(current->args);
		print_cmd_redirs(current->redirs);
		if (current->pipe_output)
			printf("    Pipes to next command\n");
		current = current->next;
		cmd_num++;
	}
}