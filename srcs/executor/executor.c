#include "executor.h"


/* BUILTIN's TO INCLUDE
 ◦ echo - with option -n
 ◦ cd - with only a relative or absolute path
 ◦ pwd - with no options
 ◦ export - with no options
 ◦ unset - with no options
 ◦ env - with no options or arguments
 ◦ exit - with no options
*/


int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2) // finché nessuna delle due stringhe è terminata
    {
        if (*s1 != *s2)
            return (unsigned char)*s1 - (unsigned char)*s2;
        s1++;
        s2++;
    }
    // se siamo usciti dal ciclo, almeno una delle due è terminata
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int execute_builtin(t_token *token)
{
	if (!token || token->type !=  WORD)
		return (0);

//printf("bella chico .%s.\n", token->value);
	if (ft_strcmp(T_ECHO, token->value) == 0)
	{
		if(ft_strcmp(T_FLAG_N, token->next->value) == 0)
		{

			printf("\n%s\n", token->next->next->value);
		}
		else
			printf("%s", token->next->value);
	}
	else if (strcmp(T_CD, token->value) == 0)
	{
		if (chdir(token->next->value) != 0)
		{
			perror("cd");
		}
	}
	else
	{
		// is not a recognized built in
		return (0);
	}

	return (1);
}

int executor(t_token *token)
{
	token = create_test_tokens(token);
	execute_builtin(token);
	return 0;
}

int main(void)
{
	t_token	token;
	executor(&token);
	return (0);
}
