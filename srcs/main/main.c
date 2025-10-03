#include "ft_lib.h"
#include "ft_sprintf.h"
#include "minishell.h"
#include "executor.h"
#include <unistd.h>

#define GREEN "\33[32m"
#define BLUE "\33[34m"
#define DEFAULT "\33[39m"

char    *get_prompt()
{
    char    *working_directory;
    char    *user;
    char    *prompt;
    size_t  prompt_len;

    user = getenv("USER");
    working_directory = getcwd(NULL, 0);
    prompt_len = ft_strlen(user) +
                 ft_strlen(working_directory) + 
                 (ft_strlen(GREEN) * 4) + 
                 3 + 1;
    prompt = malloc(sizeof(char) * prompt_len);
    ft_sprintf(prompt, "%s%s%s%c%s%s%s%c%c",
                GREEN, user, 
                DEFAULT, ':',
                BLUE, working_directory,
                DEFAULT, '$', ' ');
    free(working_directory);
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
