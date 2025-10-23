#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include <sys/types.h>
# include <dirent.h>
# include <unistd.h>
# include <stdbool.h>

# define T_ECHO "echo"
# define T_FLAG_N "-n"
# define T_CD "cd"
# define T_PWD "pwd"
# define T_EXPORT "export"
# define T_UNSET "unset"
# define T_ENV "env"
# define T_EXIT "exit"

t_token 	*create_test_tokens(t_token *token);
int			executor(t_shell *shell);
char	    *get_location(char *cmd);
int			setup_heredoc(char *delimiter);
void 		setup_pipe(t_cmd *cmd);

/* Builtins */
void	env(void);
void	echo(char **args);
void	cd(char **args);
void	pwd(void);
void	exit_shell(char **args);
#endif
