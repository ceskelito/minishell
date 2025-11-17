#include "minishell.h"

int     token_cat_to_next(t_token **token);

int    token_expand_dollars(t_token **token_list)
{
    t_token *curr;

    curr = *token_list;
    while (curr)
    {
        if (curr->expand_dollar)
        {
            curr->value = get_expanded_string(curr->value);
            ezg_add(TOKEN, curr->value);
        }
        if (!curr->value)
            return (-1);
        curr = curr->next;
    }
    curr = *token_list;
    while (curr)
    {
        if (curr->cat_to_next)
            token_cat_to_next(&curr);
        curr = curr->next;
    }
    return (0);
}

