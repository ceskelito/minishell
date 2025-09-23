#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/headers/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# define SUCCESS 0
# define ERROR 1

/*
 * Global variable to handle signals
 * As per subject: Just one global variable to store signal number
*/
extern int	g_sig_status;

/* Token types */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE
}	t_token_type;

/* Token structure */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	int				type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
	int				pipe_output;
}	t_cmd;

/* Shell environment structure */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env			*env_list;
	t_cmd			*cmd_list;
	t_token			*tokens;
	char			**env_array;
	char			*line;
	int				exit_status;
	int				interactive;
}	t_shell;

/* Function prototypes needed for main.c */
void	init_shell(t_shell *shell, char **envp);
void	setup_interactive_signals(void);
int		process_line(t_shell *shell, char *line);
void	shell_loop(t_shell *shell);
void	cleanup_shell(t_shell *shell);

/* Functions that main.c expects to exist */
t_env	*init_env(char **envp);
void	env_to_array(t_shell *shell);
void	ft_free_array(char **array);
void	free_env(t_env *env);
void	handle_sigint(int sig);
int		check_syntax(t_token *tokens);
void	expand_tokens(t_token *tokens, t_shell *shell);
t_cmd	*parse_tokens(t_token *tokens, t_shell *shell);
int		execute_cmds(t_cmd *cmds, t_shell *shell);
void	free_cmds(t_cmd *cmds);
void	ft_putendl_fd(char *s, int fd);

int		handle_dollar_sign(char *input, int *i, char **result, int *result_len);
int		is_special_in_word(char c);
char	*ft_strjoin_char(char *s1, char c);  // Нужна эта утилита
int		ft_isalnum(int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);