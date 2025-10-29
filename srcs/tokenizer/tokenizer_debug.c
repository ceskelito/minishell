#include "minishell.h"

static void	print_token_type_name(t_token_type type)
{
	if (type == WORD)
		printf("WORD");
	else if (type & PIPE)
		printf("PIPE");
	else if (type & HEREDOC)
		printf("HEREDOC");
	else if (type & IN)
		printf("IN");
	else if (type & APPEND)
		printf("APPEND");
	else if (type & OUT)
		printf("OUT");
	else if (type & AND)
		printf("AND");
	else if (type & OR)
		printf("OR");
	else if (type & P_OPEN)
		printf("P_OPEN");
	else if (type & P_CLOSE)
		printf("P_CLOSE");
	else
		printf("UNKNOWN");
}

void	print_tokens(t_token *tokens)
{
	t_token	*current;

	printf("TOKENS:\n");
	current = tokens;
	while (current)
	{
		printf("  [%s] - type: ", current->value);
		print_token_type_name(current->type);
		printf("\n");
		current = current->next;
	}
}
