#include "ft_printf.h"
#include "minishell.h"
#include "executor.h"

static char    *ft_strchrcpy(char *dest, char *src, char c)
{
        size_t  i;

        i = 0;
        if (!dest || !src)
                return (NULL);
        while (src[i] != c)
        {
                dest[i] = src[i];
                i ++;
        }
        if (c != '\0')
        {
                dest[i] = c;
                i ++;
        }
        dest[i] = '\0';
        return (dest);    
}

static void sort_strings(char *array[])
{
    char *temp;
    int i = 0;
    int size;

    size = 0;
	while (array && array[size])
	    size++;
    while (i < size - 1) {
        int j = i + 1;
        
        while (j < size) {
            if (ft_strcmp(array[i], array[j]) > 0) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
            j++;
        }
        i++;
    }
}

void    print_export()
{
    char __attribute__  ((cleanup(clean_array)))    **dest;
    char                                            *key;
    char                                            *value;
    int                                             i;

	dest = dup_array(NULL, ft_getenv_array());
	if (!dest)
	    return ;
    sort_strings(dest);
    i = 0;
    while (dest[i])
    {
        key = ft_calloc(sizeof(char), ft_strlen(dest[i]));
        ft_strchrcpy(key, dest[i], '=');
        value = ft_strchr(dest[i], '=');
        printf("declare -x \"%s%s\"\n", key, value + 1);
        free(key);
        i++;
    }
}
