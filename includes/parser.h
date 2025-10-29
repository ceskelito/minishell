typedef enum e_redir_type
{
	REDIR_IN,       // <
	REDIR_OUT,      // >
	REDIR_APPEND,   // >>
	REDIR_HEREDOC   // <<
}	t_redir_type;

/* Redirection structure */
typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

/* Command structure */
typedef struct s_cmd
{
	char			**args;         // Command arguments (NULL terminated)
	t_redir			*redirs;        // List of redirections
	struct s_cmd	*next;          // Next command in pipeline
	int				pipe_fd[2];     // Pipe file descriptors
	int				has_pipe;       // 1 if followed by pipe
}	t_cmd;

/* Parser function prototypes */
t_cmd	*parse_tokens(t_token *tokens);
t_cmd	*create_cmd(void);
t_redir	*create_redir(t_redir_type type, char *file);
void	add_redir(t_cmd *cmd, t_redir *redir);
int		add_arg(t_cmd *cmd, char *arg);
void	free_cmds(t_cmd *cmds);
void	free_cmd_args(char **args);
void	free_redirs(t_redir *redirs);