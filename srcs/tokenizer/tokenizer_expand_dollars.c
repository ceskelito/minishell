/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_dollars.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:06:08 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/19 10:34:05 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * This function will not free any memory, beacuse i attended that
 * the tokens are saved in the TOKEN list of garbage (from libezalloc)
*/
int     token_cat_to_next(t_token **token)
{
    t_token    *curr;
    t_token    *next;
    char       *new_value;

    curr = *token;
    next = curr->next;
    if (!next || !curr->cat_to_next || curr->type != next->type)
        return (-1);
    new_value = ezg_calloc(TOKEN, sizeof(char),
                    ft_strlen(curr->value) + ft_strlen(next->value) + 1);
    if (!new_value)
        return (-1);
    ft_sprintf(new_value, "%s%s", curr->value, next->value);
    curr->value = new_value;
    new_value = NULL;
    curr->cat_to_next = next->cat_to_next;
    curr->next = next->next;
    return (0);
}

int    token_expand_dollars_and_cat(t_token **token_list)
{
    t_token    *curr;

    curr = *token_list;
    while (curr)
    {
        if (curr->expand_dollar)
        {
            curr->value = string_expand_dollars(curr->value);
            ezg_add(TOKEN, curr->value);
            if (!curr->value)
                return (-1);
        }
        curr = curr->next;
    }

    // Maybe from here can be a different function
    curr = *token_list;
    while (curr)
    {
        if (curr->cat_to_next)
        {
            token_cat_to_next(&curr);
            continue;
        }
        curr = curr->next;
    }
    return (0);
}

