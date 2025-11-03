#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "ezgalloc.h" 
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
# include <stdbool.h>

# define SUCCESS 0
# define ERROR 1

# define COMMAND	"parsing"
# define GLOBAL		"global"
# define EXECUTING	"execute"
# define TOKEN		"tokenizing"

extern int	g_sig_status;

/* Token types */
typedef enum e_token_type
{
	WORD = 1 << 0,
	PIPE = 1 << 1,
	IN = 1 << 2,
	OUT = 1 << 3,
	APPEND = 1 << 4 | OUT,
	HEREDOC = 1 << 5 | IN,
	AND = 1 << 6,
	OR = 1 << 7,
	P_OPEN = 1 << 8,
	P_CLOSE = 1 << 9,
	BUILT = 1 << 10
}	t_token_type;

/* Token structure */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	bool			expand_dollar;
	struct s_token	*next;
}	t_token;

/* Redirection structure */
typedef struct s_redir
{
	t_token_type	type;
	int				pipe_fd;
	char			*file;
	struct s_redir	*next;
}	t_redir;

/* Command structure */
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

/* Main shell structure */
typedef struct s_shell
{
	t_env			*env_list;
	t_cmd			*cmd_list;
	t_token			*tokens;
	char			**env_array;
	char			*line;
	int				exit_status;
	int				interactive;
	int				std_out;
	int				std_in;
}	t_shell;

/* ========================================================================== */
/*                              TOKENIZER                                     */
/* ========================================================================== */

t_token			*create_token(char *value, t_token_type type);
void			add_token(t_token **head, t_token *new_token);
t_token			*tokenize_input(char *input);
t_token_type	get_token_type(char *input);
int				fill_word_token(t_token *token, char *input);

/* Tokenizer utilities */
void			handle_dollar_sign(char *input, int *i, char **result);
// char			*process_quotes(char *input, int *i, char *result);
char			*ft_strjoin_char(char *s1, char c);
char			*get_operator_value(char *input, t_token_type type);

/* ========================================================================== */
/*                                PARSER                                      */
/* ========================================================================== */

t_cmd			*parse_tokens(t_token *tokens);
t_redir			*create_redir(int type, char *file);
void			add_redir(t_cmd *cmd, t_redir *redir);

/* Parser utilities */
int				is_redir_token(t_token_type type);
int				parse_redirection(t_cmd *cmd, t_token **token);

/* Parser cleanup */
void			free_cmds(t_cmd *cmds);
void			free_cmd_args(char **args);
void			free_redirs(t_redir *redirs);
void			free_single_cmd(t_cmd *cmd);
void			cleanup_parsing(t_shell *shell);

/* Debug functions */
void			print_tokens(t_token *tokens);
void			print_cmd_list(t_cmd *cmd_list);

/* ========================================================================== */
/*                           VARIABLE EXPANSION                               */
/* ========================================================================== */

char			*expand_variables(char *str, bool expand_flag);
void			expand_token_list(t_token *tokens);

/* ========================================================================== */
/*                           SYNTAX VALIDATION                                */
/* ========================================================================== */

int				check_unclosed_quotes(char *input);
int				check_pipe_syntax(t_token *tokens);
int				check_redir_syntax(t_token *tokens);
int				validate_syntax(char *input, t_token *tokens);

/* ========================================================================== */
/*                           ERROR MANAGEMENT                                 */
/* ========================================================================== */

typedef enum e_error_type
{
	ERR_SYSTEM,
	ERR_INTERNAL,
	ERR_SYNTAX
} t_error_type;

void			handle_system_error(char *context, char *filename);
void			handle_internal_error(char *context, char *details);
void			handle_syntax_error(char *token, char *message);
void			handle_command_not_found(char *command);
void			handle_execution_error(char *command, t_error_type error_type);
void			handle_parsing_error(char *context, t_error_type error_type);

/* ========================================================================== */
/*                        ENVIRONMENT VARIABLES                               */
/* ========================================================================== */

char			*env_get(char *key);
char			*env_get_safe(char *key, char *default_val);
int				env_set(char *key, char *value);
int				env_unset(char *key);
bool			env_exists(char *key);
bool			validate_env_name(char *name);
int				env_export(char *assignment);
void			env_print_all(void);

/* ========================================================================== */
/*                              MAIN & UTILS                                  */
/* ========================================================================== */

void			init_shell(t_shell *shell, char **envp);
int				process_line(t_shell *shell);
void			shell_loop(t_shell *shell);
void			cleanup_shell(t_shell *shell);
void			set_exit_status(int value);
int				get_exit_status(void);

/* Error handling */
void			ft_error(char *str, int n);
int				syntax_error(char *token);
void			print_error(char *cmd, char *msg);

/* Memory utilities */
void			ft_free_array(char **array);
void			free_shell(void *shell);
char			*ft_strjoin_three(char *s1, char *s2, char *s3);

/* String utilities (if not in libft) */
int				ft_isalnum(int c);
char			*ft_substr(char const *s, unsigned int start, size_t len);
void			ft_putendl_fd(char *s, int fd);

#endif
