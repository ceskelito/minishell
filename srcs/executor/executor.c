#include "executor.h"
#include "ft_lib.h"


/* BUILTIN's TO INCLUDE
 ◦ |DID| echo - with option -n
 ◦ |DID| cd - with only a relative or absolute path
 ◦ |DID| pwd - with no options
 ◦ export - with no options
 ◦ unset - with no options
 ◦ |DID| env - with no options or arguments
 ◦ exit - with no options
*/

// static void p_free_arr(char ***p) {
//     if (*p != NULL) {
//         for (int i = 0; *p[i] != NULL; i++) {
//             free(*p[i]);
//         }
//         free(*p);
//     }
// }

// static void	p_free(char **p)
// {
// 	free(*p);
// }

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

	if (ft_strcmp(T_ECHO, token->value) == 0)
	{
		if(ft_strcmp(T_FLAG_N, token->next->value) == 0)
			echo(token->next->next->value, true);
		else
			echo(token->next->value, false);
	}
	else if (strcmp(T_CD, token->value) == 0)
	{
		if (!token->next)
			change_directory(NULL);
		else
			change_directory(token->next->value);
	}
	else if (strcmp(T_PWD, token->value) == 0)
		print_workig_directory();
	else if (strcmp(T_ENV, token->value) == 0)
		env();
	else
	{
		// is not a recognized built in
		return (0);
	}

	return (1);
}

char **create_args(t_token *token)
{
	char *string;
	char *tmp;
	
	string = NULL;
	tmp = NULL;
	while(token && token->type == WORD)
	{
		tmp = string;
		string = ft_strjoin_multi(3, string, " ", token->value);
		free(tmp);
		tmp = NULL;
	}
	return (ft_split(string, ' '));
}

char	*get_location(char *cmd);

int executor(t_token *token)
{
	extern char									**environ;
	//char __attribute__ ((cleanup (p_free_arr)))	**cmd;
	//char __attribute__ ((cleanup (p_free)))		*location;
	char	**cmd;
	char	*location;
	
	if (execute_builtin(token))
		return (0);
	cmd = create_args(token);
	location = get_location(cmd[0]);
	if (!location)	
		return (1);
	execve(ft_strjoin(location, cmd[0]), cmd, environ);
	//token = create_test_tokens(token);
	return (0);
}

//int main(void)
//{
//	t_token	token;
//	executor(&token);
//	return (0);
//}
