/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_concatenate.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:06:08 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/24 17:27:00 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** This function will not free any memory, because the tokens are saved
** in the TOKEN garbage list of libezalloc.
*/
static int	token_cat_to_next(t_token *curr)
{
	t_token	*next;
	char	*new_value;
	size_t	len;

	if (!curr || !curr->next || !curr->cat_to_next
		|| curr->type != curr->next->type)
		return (-1);
	next = curr->next;
	len = ft_strlen(curr->value) + ft_strlen(next->value) + 1;
	new_value = ezg_calloc(TOKEN, len, sizeof(char));
	if (!new_value)
		return (-1);
	ft_sprintf(new_value, "%s%s", curr->value, next->value);
	curr->value = new_value;
	curr->cat_to_next = next->cat_to_next;
	curr->next = next->next;
	return (0);
}

int	concatenate_tokens(t_token **token_list)
{
	t_token	*curr;

	if (!token_list || !*token_list)
		return (0);
	curr = *token_list;
	while (curr)
	{
		if (curr->cat_to_next)
		{
			if (token_cat_to_next(curr) < 0)
				return (-1);
			continue ;
		}
		curr = curr->next;
	}
	return (0);
}
