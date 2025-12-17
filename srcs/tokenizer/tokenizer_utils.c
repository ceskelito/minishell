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

static t_token_type	check_double_operators(char *input)
{
	if (input[0] == '|' && input[1] == '|')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `||'\n");
		return (0);
	}
	if (input[0] == '&' && input[1] == '&')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `&&'\n");
		return (0);
	}
	if (input[0] == '<' && input[1] == '<')
		return (HEREDOC | IN);
	if (input[0] == '>' && input[1] == '>')
		return (APPEND | OUT);
	return (-1);
}

static t_token_type	check_single_operators(char c)
{
	if (c == '|')
		return (PIPE);
	if (c == '<')
		return (IN);
	if (c == '>')
		return (OUT);
	if (c == '(')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `('\n");
		return (0);
	}
	if (c == ')')
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `)'\n");
		return (0);
	}
	return (-1);
}

t_token_type	get_token_type(char *input)
{
	t_token_type	type;

	type = check_double_operators(input);
	if (type != -1)
		return (type);
	type = check_single_operators(input[0]);
	if (type != -1)
		return (type);
	return (WORD);
}

char	*get_operator_value(char *input, t_token_type type)
{
	if (type & (HEREDOC | APPEND))
		return (ezg_add(TOKEN, ft_substr(input, 0, 2)));
	else
		return (ezg_add(TOKEN, ft_substr(input, 0, 1)));
}
