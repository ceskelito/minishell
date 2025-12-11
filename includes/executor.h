#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include <sys/types.h>
# include <dirent.h>
# include <unistd.h>
# include <stdbool.h>

void		executor(t_shell *shell);
bool		resolve_command_location(t_cmd *cmd);
void		setup_heredocs(t_cmd *cmd_list);
int		    open_pipeline_fds(t_cmd *cmd_list);

/* Redirections */
int		apply_redirs(t_redir *r);
void	close_pipe_fds(t_cmd *cmd, int redir_type, int cmd_to_parse);

/* Builtins */
void	env(void);
void	echo(char *const args[]) ;
void	cd(char *const args[]) ;
void	pwd(void);
void	exit_shell(char *const args[]) ;
void	export(char *const args[]) ;
void	unset(char *const args[]) ;
#endif
