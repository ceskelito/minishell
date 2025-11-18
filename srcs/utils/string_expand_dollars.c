/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand_dollars.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:15:36 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/17 20:06:25 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"

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

void fill_chunks(char **splitted, char *str)
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
    splitted[n] = NULL;
}

static char **ft_split_in_chunks(char *str, char delimiter)
{
    int     delimiter_count;
    char    **splitted;
        
    delimiter_count = count_char(str, delimiter);
    if (!delimiter_count)
    {
        splitted = ft_calloc(sizeof(char *), 2);
        splitted[0] = ft_strdup(str);
    }
    else
    {
        splitted = ft_calloc(delimiter_count * 2 + 1, sizeof(char *));
        fill_chunks(splitted, str);
    }
    return (splitted);
}

char    *string_expand_dollars(char *old)
{
    int                                             i;
    int                                             new_len;
    char                                            *new;
    char                                            *temp;
    char __attribute__((cleanup(clean_array)))      **splitted;

    splitted = NULL;
    if (!ft_strchr(old, '$'))
        return (ft_strdup(old));
    splitted = ft_split_in_chunks(old, '$');
    new_len = 0;
    i = 0;
    while (splitted[i])
    {
        if (splitted[i][0] == '$' && splitted[i][1] && ft_isalpha(splitted[i][1]))
        {
            temp = ft_getenv(&(splitted[i][1]));
            free(splitted[i]);
            if (!temp)
                temp = "";
            splitted[i] = ft_strdup(temp);
            temp = NULL;
        }
        // printf("split[%d] = %s\n", i, splitted[i]);//DEBUG
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
