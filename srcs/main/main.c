#include "minishell.h"

int main(void)
{
    char    *input;
    t_token *tokens;
    t_token *tmp;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break ;
        if (*input)
            add_history(input);

        tokens = tokenize_input(input);

        // выводим токены для отладки
        tmp = tokens;
        while (tmp)
        {
            printf("TOKEN: %-15s | TYPE: %d\n", tmp->value, tmp->type);
            tmp = tmp->next;
        }

        free_tokens(tokens);
        free(input);
    }
    return (0);
}