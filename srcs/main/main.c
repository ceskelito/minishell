#include "minishell.h"
#include "executor.h"
#include <unistd.h>

char    *get_prompt()
{
    char    *working_directory;
    char    *user;
    char    *prompt;

    user = getenv("USER");
    working_directory = getcwd(NULL, 0);
    prompt = ft_strdup(ft_strjoin_multi(4 ,user, ":\0", working_directory, "> \0"));
    free(working_directory);
    // free(user);
    return (prompt);
}

int main(void)
{
    // extern char    **environ;
    char           *input;
    t_token        *tokens;
    // t_token        *tmp;

    while (1)
    {
        input = readline(get_prompt());
        if (!input)
            break ;
        if (*input)
            add_history(input);

        tokens = tokenize_input(input);

        // выводим токены для отладки
        // tmp = tokens;
        // while (tmp)
        // {
        //     printf("TOKEN: %-15s | TYPE: %d\n", tmp->value, tmp->type);
        //     tmp = tmp->next;
        // }
        executor(tokens);
        free_tokens(tokens);
        free(input);
    }
    return (0);
}
