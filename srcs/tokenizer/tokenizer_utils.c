#include "minishell.h"

/*
 * add_token - Adds a token to the end of the linked list
 */
void add_token(t_token **head, t_token *new_token)
{
	t_token *curr;

	if (!head || !new_token)
		return;
		
	if (!*head)
	{
		*head = new_token;
		return;
	}
	
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new_token;
}

/*
 * get_token_type - Determines token type from input characters
 * DEBUG VERSION - with trace output
 */
t_token_type get_token_type(char *input)
{
	if (!input || !*input)
		return (WORD);
	
	printf("DEBUG get_token_type: input='%c%c'\n", input[0], input[1] ? input[1] : '?');
	
	// Check two-character operators FIRST
	if (input[0] == '>' && input[1] == '>')
	{
		printf("DEBUG: Found >> -> returning APPEND\n");
		return (APPEND);
	}
	if (input[0] == '<' && input[1] == '<')
	{
		printf("DEBUG: Found << -> returning HEREDOC\n");
		return (HEREDOC);
	}
	if (input[0] == '&' && input[1] == '&')
	{
		printf("DEBUG: Found && -> returning AND\n");
		return (AND);
	}
	if (input[0] == '|' && input[1] == '|')
	{
		printf("DEBUG: Found || -> returning OR\n");
		return (OR);
	}
	
	// Then single-character operators
	if (input[0] == '>')
	{
		printf("DEBUG: Found > -> returning OUT\n");
		return (OUT);
	}
	if (input[0] == '<')
	{
		printf("DEBUG: Found < -> returning IN\n");
		return (IN);
	}
	if (input[0] == '|')
	{
		printf("DEBUG: Found | -> returning PIPE\n");
		return (PIPE);
	}
	if (input[0] == '(')
	{
		printf("DEBUG: Found ( -> returning P_OPEN\n");
		return (P_OPEN);
	}
	if (input[0] == ')')
	{
		printf("DEBUG: Found ) -> returning P_CLOSE\n");
		return (P_CLOSE);
	}
	
	printf("DEBUG: Default -> returning WORD\n");
	return (WORD);
}

/*
 * get_operator_value - Returns string value of operator token
 */
char *get_operator_value(char *input, t_token_type type)
{
	if (!input)
		return (NULL);
	
	switch (type)
	{
		case APPEND:
			return (ezg_add(TOKEN, ft_strdup(">>")));
		case HEREDOC:
			return (ezg_add(TOKEN, ft_strdup("<<")));
		case AND:
			return (ezg_add(TOKEN, ft_strdup("&&")));
		case OR:
			return (ezg_add(TOKEN, ft_strdup("||")));
		case OUT:
			return (ezg_add(TOKEN, ft_strdup(">")));
		case IN:
			return (ezg_add(TOKEN, ft_strdup("<")));
		case PIPE:
			return (ezg_add(TOKEN, ft_strdup("|")));
		case P_OPEN:
			return (ezg_add(TOKEN, ft_strdup("(")));
		case P_CLOSE:
			return (ezg_add(TOKEN, ft_strdup(")")));
		default:
			return (NULL);
	}
}

/*
 * is_operator_char - Checks if character is part of an operator
 */
int	is_operator_char(char c)
{
	return (ft_strchr("|<>&()", c) != NULL);
}

/*
 * skip_whitespace - Skips whitespace characters from given position
 */
int	skip_whitespace(char *str, int start)
{
	int i;
	
	i = start;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

/*
 * get_operator_length - Returns operator length (1 or 2 characters)
 */
int	get_operator_length(char *input)
{
	if (!input || !*input)
		return (0);
	
	// Two-character operators
	if ((input[0] == '>' && input[1] == '>') ||
		(input[0] == '<' && input[1] == '<') ||
		(input[0] == '&' && input[1] == '&') ||
		(input[0] == '|' && input[1] == '|'))
		return (2);
	
	// Single-character operators
	if (is_operator_char(input[0]))
		return (1);
	
	return (0);
}