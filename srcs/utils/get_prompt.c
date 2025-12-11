/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 10:57:44 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 12:16:07 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*prompt_getcwd(char *home_symbol)
{
	char	*home_path;
	char	*new_directory;
	char	*directory;

	directory = getcwd(NULL, 0);
	if (!directory)
		return (perror("minishell"), NULL);
	home_path = ft_getenv("HOME");
	if (!home_path)
		return (directory);
	if (ft_strnstr(directory, home_path, ft_strlen(home_path)))
	{
		new_directory = ft_calloc(ft_strlen(home_symbol) + ft_strlen(directory
					+ ft_strlen(home_path)) + 1, sizeof(char));
		if (!new_directory)
			return (directory);
		ft_sprintf(new_directory, "%s%s", home_symbol, directory
			+ ft_strlen(home_path));
		free(directory);
		return (new_directory);
	}
	return (directory);
}

static char	*get_prompt(void)
{
	char	*working_directory;
	char	*user;
	char	*prompt;
	size_t	prompt_len;

	user = ft_getenv("USER");
	working_directory = prompt_getcwd(HOME_SYMBOL);
	if (!user)
		user = "\0";
	if (!working_directory)
		working_directory = "\0";
	prompt_len = ft_strlen(user) + ft_strlen(working_directory)
		+ (ft_strlen(GREEN) * 4) + 3 + 1;
	prompt = malloc(sizeof(char) * prompt_len);
	ft_sprintf(prompt, "%s%s%s%c%s%s%s%c%c", GREEN, user, DEFAULT, ':', BLUE,
		working_directory, DEFAULT, '$', ' ');
	free(working_directory);
	ezg_add(EXECUTING, prompt);
	return (prompt);
}

void readline_on_buff(char **buff)
{
	if (!buff)
		return;
	*buff = readline(get_prompt());
	ezg_add(EXECUTING, *buff);
}
