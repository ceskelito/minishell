#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"
#include "executor.h"
#include <dirent.h>

void	free_path(char ***path)
{
	int	i;

	i = 0;
	if (!(*path))
		return;
	while((*path)[i])
		free((*path)[i++]);
	free(*path);
}

/**
 * lookup_command_path - Search for a command in the directories listed in $PATH
 *
 * @cmd: Name of the command to search for
 *
 * Return: A newly allocated string containing the absolute path of the command;
 *		   NULL if the command is not found or on failure.
 *
 * Note: This function does not handle commands that already contain a '/'.
 */
static char	*lookup_command_path(char *cmd)
{
	char __attribute__	((cleanup(free_path)))	**path;
	DIR											*dir;
	struct dirent 								*entry;
	int											i;

	if (!cmd)
		return (NULL);
	path = ft_split(getenv("PATH"), ':');
	if (!path)
		return (NULL);
	i = -1;
	while (i++, path[i])
	{
		dir = opendir(path[i]);
		if (!dir)
			continue;
		entry = readdir(dir);
		while (entry)
		{
			if (ft_strcmp(entry->d_name, cmd) == 0)
			{
				closedir(dir);
				return (ezg_add(EXECUTING, ft_strjoin_multi(3, path[i], "/", cmd)));
			}
			entry = readdir(dir);
		}
		closedir(dir);
	}
	return (NULL);
}

/**
 * resolve_command_path - Set the location and args[0] of a command
 *
 * @cmd: Pointer to the t_cmd structure representing the command
 *
 * If cmd->args[0] contains a '/' (absolute or relative path):
 *   - cmd->location is set to point to the original string
 *   - cmd->args[0] is replaced by a newly allocated string containing
 *     only the command name (after the last '/')
 *
 * If cmd->args[0] does not contain '/':
 *   - lookup_command_path() is used to find the full path in $PATH
 *   - cmd->location is set to the result (newly allocated string or NULL if not found)
 */

void	resolve_command_path(t_cmd *cmd)
{
	char	*slash;

	slash = ft_strrchr(cmd->args[0], '/');
	if (!slash)
	{
		cmd->location = lookup_command_path(cmd->args[0]);
		return ;
	}
	cmd->location = cmd->args[0];
	cmd->args[0] = ft_strdup(slash + 1);
	ezg_add(EXECUTING, cmd->args[0]);
}
