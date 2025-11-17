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

static int    count_char(char *s, char c)
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

// static void helper(char *str, char **split, int *i, int *n, int len)
// {  
//     split[*n] = ft_substr(str, *i, len);
//     (*n)++;
//     split[*n] = NULL;
//     (*i) += len;
// }

static void fill_chunks(char **splitted, char *str)
{
    int    i;
    int    len;
    int    n;

    i = 0;
    n = 0;
    while (str[i])
    {
        len = 0;
        while (str[i + len] && str[i + len] != '$')
            len++;
        splitted[n] = ft_substr(str, i, len);
        n++;
        i += len;
        len = 1;
        while (str[i] && ft_isalnum(str[i + len]))
            len++;
        splitted[n] = ft_substr(str, i, len);
        n++;
        i += len;
    }
}

static char **ft_split_in_chunks(char *str, char delimiter)
{
    int     delimiter_count;
    char    **splitted;
        
    delimiter_count = count_char(str, delimiter);
    if (!delimiter_count)
        return ((char **)str);
    splitted = ft_calloc(delimiter_count * 2 + 1, sizeof(char *));
    fill_chunks(splitted, str);
    return (splitted);
}

char    *get_expanded_string(char *old)
{
    int                                             i;
    int                                             new_len;
    char                                            *new;
    // char                                            *temp;
    // char __attribute__((cleanup(clean_array)))      **splitted;
    char **splitted;
    
    if (!ft_strchr(old, '$'))
        return (old);
    splitted = ft_split_in_chunks(old, '$');
    new_len = 0;
    i = 0;
    while (splitted[i])
    {
        if (splitted[i][0] == '$' && splitted[i][1])
        {
            // free(splitted[i]);
            // temp = ft_getenv(&(splitted[i][1]));
            splitted[i] = ft_getenv(&(splitted[i][1]));//ft_strdup(temp);
            // temp = NULL;
        }
        printf("split[%d] = %s\n", i, splitted[i]);//DEBUG
        new_len += ft_strlen(splitted[i]);
        i++;
    }
    new = ft_calloc(new_len, sizeof(char));
    i = 0;
    while (splitted[i])
    {
        ft_strlcat(new, splitted[i], new_len);
        i++;
    }
    return (new);
}

// void    expand_tokens(t_token **token_list)
// {
//     t_token *curr;

//     ezg_group_create("expansion");
//     curr = *token_list;
//     while (curr)
//     {
//         if (curr->expand_dollar)
//             curr->value = get_expanded_string(curr->value);
//         curr = curr->next;
//     }
//     curr = *token_list;
//     while (curr)
//     {
//         if (curr->cat_to_next)
//             token_cat_to_next(&curr);
//         curr = curr->next;
//     }
//     ezg_group_release("expansion");
// }
