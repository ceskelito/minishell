#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
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
	WORD = 0,
	PIPE = 1 << 1,
	IN = 1 << 2,
	OUT = 1 << 3,
	APPEND = 1 << 4,
	HEREDOC = 1 << 5,
	AND = 1 << 6,
	OR = 1 << 7,
	P_OPEN = 1 << 8,
	P_CLOSE = 1 << 9,
	BUILT = 1 << 10
}	t_token_type;

//typedef enum e_type
//{
//	NONE = 0,
//	CMD = 1 << 1,
//	LIMITER = 1 << 2,
//	DELIMETER = 1 << 3,
//	REDIRECT = 1 << 4,
//	FILENAME = 1 << 5,
//	AND = 1 << 6,
//	OR = 1 << 7,
//	IN = 1 << 8,
//	OUT = 1 << 9,
//	HEREDOC = 1 << 10,
//	APPEND = 1 << 11,
//	PIPE = 1 << 12,
//	OPEN = 1 << 13,
//	CLOSE = 1 << 14,
//	NEW_LINE = 1 << 15,
//	END = 1 << 16,
//}	t_type;

/* Token structure */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/* Tokenizer */
 t_token	*tokenize_input(char *input);
 t_token_type	identify_token_type(char *input, int *i);
 char			*process_quotes(char *input, int *i, char *result);
 char			*extract_word(char *input, int *i);
 int				is_special_in_word(char c);
 void			handle_dollar_sign(char *input, int *i, char **result);
 char			*ft_strjoin_char(char *s1, char c);  // Нужна эта утилита
 char			*ft_substr(char const *s, unsigned int start, size_t len);
 void		free_tokens(t_token *tokens);
 void		setup_signals(int n);
 void		ft_error(char *str, int n);
 void		free_shell(void	 *shell);	

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
int		process_line(t_shell *shell);
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

//int		handle_dollar_sign(char *input, int *i, char **result, int *result_len);
int		is_special_in_word(char c);
char	*ft_strjoin_char(char *s1, char c);  
int		ft_isalnum(int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);

#endif
