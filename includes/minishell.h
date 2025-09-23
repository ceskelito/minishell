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

<<<<<<< HEAD
/* Tokenizer */
t_token	*tokenize_input(char *input);
/* Token Utils */
t_token_type	identify_token_type(char *input, int *i);
char			*extract_word(char *input, int *i);

#endif
=======
int		handle_dollar_sign(char *input, int *i, char **result, int *result_len);
int		is_special_in_word(char c);
char	*ft_strjoin_char(char *s1, char c);  // Нужна эта утилита
int		ft_isalnum(int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
>>>>>>> origin/tokenizer
