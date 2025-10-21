#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_lib.h"
#include "libft.h"

void p_free_arr(char ***p) {
    if (*p != NULL) {
        for (int i = 0; *p[i] != NULL; i++) {
            free(*p[i]);
        }
        free(*p);
    }
}

void	p_free(char **p)
{
	free(*p);
}

void	p_closedir(DIR **dir)
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
				return (ft_strjoin(path[i], "/"));
		}
		i++;
	}
	return (NULL);
}
