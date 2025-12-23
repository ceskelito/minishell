/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:24:09 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/08 17:24:12 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*temp;

	if (!*head)
		*head = new_token;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
}

static int	check_double_operators(char *input, t_token_type *result)
{
	if (input[0] == '|' && input[1] == '|')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `||'\n");
		*result = 0;
		return (1);
	}
	if (input[0] == '&' && input[1] == '&')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `&&'\n");
		*result = 0;
		return (1);
	}
	if (input[0] == '<' && input[1] == '<')
		*result = (HEREDOC | IN);
	else if (input[0] == '>' && input[1] == '>')
		*result = (APPEND | OUT);
	else
		return (0);
	return (1);
}

static int	check_single_operators(char c, t_token_type *result)
{
	if (c == '|')
		*result = PIPE;
	else if (c == '<')
		*result = IN;
	else if (c == '>')
		*result = OUT;
	else if (c == '(')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `('\n");
		*result = 0;
	}
	else if (c == ')')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `)'\n");
		*result = 0;
	}
	else
		return (0);
	return (1);
}

t_token_type	get_token_type(char *input)
{
	t_token_type	result;

	if (check_double_operators(input, &result))
		return (result);
	if (check_single_operators(input[0], &result))
		return (result);
	return (WORD);
}

char	*get_operator_value(char *input, t_token_type type)
{
	if (type & (HEREDOC | APPEND))
		return (ezg_add(TOKEN, ft_substr(input, 0, 2)));
	else
		return (ezg_add(TOKEN, ft_substr(input, 0, 1)));
}
