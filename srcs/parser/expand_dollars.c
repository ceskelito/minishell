/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:15:36 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/14 17:27:15 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"

#define EXP "expansion"

char    *get_expanded_string(char *str);
int     token_cat_to_next(t_token **token);

int    count_char(char *s, char c)
{
    int    i;
    int    count;

    i = 0;
    count = 0;
    while (s[i])
    {
        if (s[i] == c)
            count++;
        i++;        
    }
    return (count);
}

void fill_chunks(char *old, char **spliited);

char    *get_expanded_string(char *old)
{
    char    *new;
    char    **splitted;
    int    dollars_count;
    // int    **chunks_index;
    
    dollars_count = count_char(old, '$');   
    if (!dollars_count)
        return (old);
    splitted = ezg_calloc(EXP, sizeof(char *), dollars_count * 2 + 1);
    
    return (new);
}

void    expand_tokens(t_token **token_list)
{
    t_token *curr;

    ezg_group_create("expansion");
    curr = *token_list;
    while (curr)
    {
        if (curr->expand_dollar)
            curr->value = get_expanded_string(curr->value);
        curr = curr->next;
    }
    curr = *token_list;
    while (curr)
    {
        if (curr->cat_to_next)
            token_cat_to_next(&curr);
        curr = curr->next;
    }
    ezg_group_release("expansion");
}
