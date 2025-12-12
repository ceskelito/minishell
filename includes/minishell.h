/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 16:32:31 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 16:32:33 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "ezgalloc.h"
# include "libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1

// ezalloc groups
# define COMMAND "parsing"
# define GLOBAL "global"
# define EXECUTING "execute"
# define TOKEN "tokenizing"
# define ENV "environment"

// prompt
# ifndef HOME_SYMBOL
#  define HOME_SYMBOL "~"
# endif
# define GREEN "\33[32m"
# define BLUE "\33[34m"
# define DEFAULT "\33[39m"

/* Token types */
typedef enum e_token_type
{
	WORD = 1 << 0,
	PIPE = 1 << 1,
	IN = 1 << 2,
	OUT = 1 << 3,
	APPEND = 1 << 4,
	HEREDOC = 1 << 5,
	AND = 1 << 6,
	OR = 1 << 7,
	P_OPEN = 1 << 8,
	P_CLOSE = 1 << 9,
	BUILT = 1 << 10,
}					t_token_type;

/* Token structure */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	bool			cat_to_next;
	bool			collapse_spaces;
	struct s_token	*next;
}					t_token;

/* Redirection structure */
typedef struct s_redir
{
	t_token_type	type;
	int				pipe_fd;
	char			*file;
	struct s_redir	*next;
}					t_redir;

/* Command structure */
typedef struct s_cmd
{
	char			**args;
	char			*location;
	t_redir			*redirs;
	struct s_cmd	*next;
	int				pipe_output;
}					t_cmd;

/* Main shell structure */
typedef struct s_shell
{
	t_cmd			*cmd_list;
	t_token			*tokens;
	char			**env_array;
	int				exit_status;
	int				std_out;
	int				std_in;
}					t_shell;

/* ========================================================================== */
/*                              TOKENIZER                                     */
/* ========================================================================== */

t_token				*create_token(char *value, t_token_type type);
void				add_token(t_token **head, t_token *new_token);
t_token				*tokenize_input(char *input);
t_token_type		get_token_type(char *input);
int					fill_word_token(t_token *token, char *input);
int					fill_eof_token(t_token *new, char *input);

/* Tokenizer utilities */
int					concatenate_tokens(t_token **token_list);
char				*string_expand_dollars(char *str);
void				handle_dollar_sign(char *input, int *i, char **result);
char				*process_quotes(char *input, int *i, char *result);
char				*ft_strjoin_char(char *s1, char c);
char				*get_operator_value(char *input, t_token_type type);
int					process_word_token(char *input, int *i, t_token **tokens);

/* ========================================================================== */
/*                                PARSER                                      */
/* ========================================================================== */

t_cmd				*parse_tokens(t_token *tokens);
t_redir				*create_redir(int type, char *file);
void				add_redir(t_cmd *cmd, t_redir *redir);

/* Parser utilities */
int					is_redir_token(t_token_type type);
int					parse_redirection(t_cmd *cmd, t_token **token);
int					token_count_args(t_token *token);
int					set_cmd_args(t_cmd *cmd, t_token *token);
int					go_next_cmd(t_cmd **cmd);
void				add_pipe_redir(t_cmd *cmd, t_token_type type);

/* Parser command handlers */
int					handle_word_token(t_cmd *cmd, t_token **token);
int					handle_redir_token(t_cmd *cmd, t_token **token);
int					handle_pipe_token(t_cmd **cmd, t_token *token);

/* Parser cleanup */
void				free_cmds(t_cmd *cmds);
void				free_cmd_args(char **args);
void				free_redirs(t_redir *redirs);
void				free_single_cmd(t_cmd *cmd);
void				cleanup_parsing(t_shell *shell);



/* ========================================================================== */
/*                               EXECUTOR                                     */
/* ========================================================================== */

/* Main Function*/
void				executor(t_shell *shell);

/* Builtins */
void				env(void);
void				echo(char *const args[]);
void				cd(char *const args[]);
void				pwd(void);
void				exit_shell(char *const args[]);
void				export(char *const args[]);
void				unset(char *const args[]);

/* Redirections */
int					apply_redirs(t_redir *r);
void				setup_heredocs(t_cmd *cmd_list);
int					open_pipeline_fds(t_cmd *cmd_list);
void				close_pipe_fds(t_cmd *cmd, int redir_type,
						int cmd_to_parse);

/* Other */
bool				resolve_command_location(t_cmd *cmd);

/* ========================================================================== */
/*                              MAIN & UTILS                                  */
/* ========================================================================== */

void				init_shell(t_shell *shell);
void				set_exit_status(int value);
int					get_exit_status(void);
bool				is_builtin(char *cmd);
int					count_cmds(t_cmd *cmd_list);

/* Error handling */
int					print_error(char *msg, char *err);

/* Memory utilities */
char				*ft_strjoin_three(char *s1, char *s2, char *s3);

/* String utilities (if not in libft) */
char				**dup_array(char *group, char **srcs);
char				**expand_array(char *group, char **array, int nmemb,
						int increment);

/* Automatic cleanup functions (for cleanup attribute) */
void				clean_char(char **c);
void				clean_array(char ***s);

/* === Environment Variables Managing === */

/* ========================================================================== */
/*                              ENVIRONMENT                                   */
/* ========================================================================== */

/**
 * ft_getenv - Retrieve the value of an environment variable.
 *
 * Searches for a variable named @key in the environment array and returns
 * a pointer to the value portion (the substring following '=').
 *
 * @key  Name of the variable to look up.
 *
 * Return: A pointer to the variable's value, or NULL if not found.
 */
char				*ft_getenv(char *key);

/**
 * ft_setenv - Set or update an environment variable.
 *
 * If the variable identified by @key exists, its value is replaced with @value.
 * Otherwise,
	a new entry with the format "key=value" is appended to the environment array.
 *
 * @key    Variable name to set.
 * @value  New value to assign.
 */
int					ft_setenv(char *key, char *value);

/**
 * ft_getenv_array - Retrieve the environment array.
 *
 * Return: A pointer to the NULL-terminated array of "key=value" strings
 *         representing the current environment.
 */
char				**ft_getenv_array(void);
void				ft_unsetenv(char *key);

#endif
