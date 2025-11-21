/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command_location.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:52 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/21 12:28:30 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <dirent.h>

/**
 * search_in_dir - Search for a command inside a specific directory
 *
 * @dirpath: Path of the directory to search in
 * @cmd:     Name of the command to look for
 *
 * Return: A newly allocated string containing the absolute path to the command
 *         if found; NULL if the directory cannot be opened or the command is
 *         not present.
 */
static char	*search_in_dir(char *dirpath, char *cmd)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(dirpath);
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (ft_strcmp(entry->d_name, cmd) == 0)
		{
			closedir(dir);
			return (ft_strjoin_multi(3, dirpath, "/", cmd));
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (NULL);
}

/**
 * lookup_command_path - Search for a command in the directories listed in $PATH
 *
 * @cmd: Name of the command to search for
 *
 * Return: A newly allocated string containing the absolute path of the command;
 *			NULL if the command is not found or on failure.
 *
 * Note: This function does not handle commands that already contain a '/'.
 */
static char	*lookup_for_command_in_path(char *cmd)
{
	int												i;
	char											*cmd_path;
	char __attribute__	((cleanup(clean_array)))	**path;

	path = ft_split(ft_getenv("PATH"), ':');
	if (!cmd || !path)
		return (NULL);
	i = -1;
	while (i++, path[i])
	{
		cmd_path = search_in_dir(path[i], cmd);
		if (cmd_path)
			return (cmd_path);
	}
	return (NULL);
}

/**
 * resolve_command_location - Set the location and args[0] of a command
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
 *
	- cmd->location is set to the result
	(newly allocated string or NULL if not found)
 */

bool	resolve_command_location(t_cmd *cmd)
{
	char	*slash;
	bool	location_was_given;

	slash = ft_strrchr(cmd->args[0], '/');
	if (slash)
	{
		location_was_given = true;
		cmd->location = cmd->args[0];
		cmd->args[0] = ft_strdup(slash + 1);
		ezg_add(EXECUTING, cmd->args[0]);
	}
	else
	{
		location_was_given = false;
		if (is_builtin(cmd->args[0]))
			cmd->location = ft_strdup("child");
		else
			cmd->location = lookup_for_command_in_path(cmd->args[0]);
		ezg_add(EXECUTING, cmd->location);
	}
	return (location_was_given);
}
