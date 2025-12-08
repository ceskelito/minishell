/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:01:11 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/08 17:01:15 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezgalloc.h"
#include "minishell.h"

static int	fill_operator_token(t_token *token, char *input)
{
	if (!token)
		return (-1);
	token->type = get_token_type(input);
	token->value = get_operator_value(input, token->type);
	return (ft_strlen(token->value));
}

t_token	*new_token(void)
{
	t_token	*new;

	new = ezg_calloc(TOKEN, sizeof(t_token), 1);
	new->expand_dollar = true;
	new->cat_to_next = false;
	return (new);
}

static int	handle_token(t_token **tokens, char *input, int i, bool *is_hd)
{
	t_token	*new;
	int		gap;

	new = new_token();
	if (!new)
		return (-1);
	if (ft_strchr("|<>", input[i]))
		gap = fill_operator_token(new, &input[i]);
	else if (*is_hd)
		gap = fill_eof_token(new, &input[i]);
	else
		gap = fill_word_token(new, &input[i]);
	if (gap == -1 || !new->value)
		return (-1);
	add_token(tokens, new);
	*is_hd = (new->type & HEREDOC);
	return (gap);
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	bool	is_hd;
	int		i;
	int		gap;

	tokens = NULL;
	is_hd = false;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		gap = handle_token(&tokens, input, i, &is_hd);
		if (gap < 0)
			return (NULL);
		i += gap;
	}
	return (tokens);
}
