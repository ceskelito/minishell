/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:47:26 by rceschel          #+#    #+#             */
/*   Updated: 2025/02/18 13:12:29 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char		buff[1024 + 1][BUFFER_SIZE + 1];
	char			*line;
	int				r;

	if (fd < 0 || fd > 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	ft_init_line(&line, buff[fd]);
	r = BUFFER_SIZE;
	while (1)
	{
		if (!line)
			break ;
		if (ft_strchr(line, '\n') || r < BUFFER_SIZE)
			return (line);
		r = read(fd, buff[fd], BUFFER_SIZE);
		if (r <= 0 && !line[0])
			break ;
		buff[fd][r] = '\0';
		line = ft_append_buff(line, buff[fd]);
	}
	if (line)
		free(line);
	return (NULL);
}

void	ft_init_line(char **line, char buff[])
{
	*line = malloc(sizeof(char) * 1025);
	if (!(*line))
		return ;
	**line = '\0';
	ft_strlcpy(*line, buff, BUFFER_SIZE);
	*buff = '\0';
	*line = ft_append_buff(*line, buff);
}

char	*ft_append_buff(char *line, char buff[])
{
	char	*new_line;
	int		i;

	new_line = ft_strjoin(line, buff);
	i = 0;
	*buff = '\0';
	if (ft_strchr(new_line, '\n'))
	{
		i = 0;
		while (new_line[i] != '\n')
			i++;
		i++;
		ft_strlcpy(buff, new_line + i, BUFFER_SIZE);
		new_line[i] = '\0';
	}
	free(line);
	return (new_line);
}
