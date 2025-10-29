#include "minishell.h"
#include "executor.h"

static void	p_closedir(DIR **dir)
{
	closedir(*dir);
}

char	*get_location(char *cmd)
{
	char										**path;
	DIR __attribute__ ((cleanup (p_closedir)))	*dir;
	struct dirent 								*entry;
	int											i;

	if (!cmd)
		return (NULL);
	path = ft_split(getenv("PATH"), ':');
	if (!path)
		return (NULL);
	i = 0;
	while (path[i])
		ezg_add(EXECUTING, path[i++]);
	ezg_add(EXECUTING, path);
	i = 0;
	while (path[i])
	{
		dir = opendir(path[i]);
		if (dir == NULL)
		{
			i ++;
			continue;
		}
		while((entry = readdir(dir)))
		{
			if (ft_strcmp(entry->d_name, cmd) == 0)
				return (ezg_add(EXECUTING, ft_strjoin(path[i], "/")));
		}
		closedir(dir);
		i++;
	}
	return (NULL);
}
