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

char	*get_location(char *cmd)
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
	i = 0;
	while (path[i])
	{
		dir = opendir(path[i]);
		if (dir == NULL)
		{
			i ++;
			continue;
		}
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
		i++;
	}
	return (NULL);
}
