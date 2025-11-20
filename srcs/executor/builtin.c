/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:41 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/07 15:44:41 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "ft_dprintf.h"
#include "ft_lib.h"
#include "ft_printf.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static bool	is_entry_valid(char *entry)
{
	int	i;

	if (!ft_isalpha(entry[0]))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", entry);
		return (false);
	}
	i = 0;
	while (entry[i] && entry[i] != '=')
	{
		if (!ft_isalnum(entry[i]))
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: export: `%c': not a valid identifier\n", entry);
			return (false);
		}
		i++;
	}
	if (!ft_strchr(entry, '='))
		return (false);
	return (true);
}

void	export(char *const args[])
{
	size_t	key_len;
	int		i;
	char	*key;
	char	*value;
	char	*entry;

	i = 0;
	while (i++, args[i])
	{
		entry = args[i];
		if (!is_entry_valid(entry))
			continue ;
		key_len = 0;
		while (entry[key_len] != '=')
			key_len++;
		key = ft_substr(entry, 0, key_len);
		value = ft_strchr(entry, '=') + 1;
		ft_setenv(key, value);
		free(key);
	}
}

void	unset(char *const args[])
{
	int	i;

	i = 0;
	while (i++, args[i])
	{
		ft_unsetenv(args[i]);
	}
}

void	env(void)
{
	char	**env_array;
	int		i;

	env_array = ft_getenv_array();
	i = 0;
	while (env_array[i])
	{
		ft_printf("%s\n", env_array[i]);
		i++;
	}
}

void	echo(char *const args[])
{
	int		i;
	int		j;
	bool	flag_n;

	i = 1;
	flag_n = false;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0' || j == 1)
			break ;
		flag_n = true;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!flag_n)
		ft_printf("\n");
}

void	pwd(void)
{
	ft_printf("%s\n", getcwd(NULL, 0));
}

void	cd(char *const args[])
{
	int		count;
	char	*dir;

	count = 0;
	while (args[count])
		count++;
	if (count > 2)
	{
		print_error("cd", "too many arguments\n");
		return ;
	}
	if (count == 1)
	{
		dir = ft_getenv("HOME");
		if (!dir)
		{
			print_error("cd", "HOME not set\n");
			return ;
		}
	}
	else
		dir = args[1];
	if (strcmp(dir, "") != 0)
		if (chdir(dir) != 0)
			ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", dir,
				strerror(errno));
}

void	exit_shell(char *const args[])
{
	int	status;
	int	i;

	ft_printf("exit\n");
	if (!args[1])
		exit(get_exit_status());
	i = 0;
	while (args[1][i])
	{
		if (!ft_isdigit(args[1][i]) && !(i == 0 && (args[1][i] == '+'
					|| args[1][i] == '-')))
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: exit: %s: numeric argument required\n",
				args[1]);
			exit(255);
		}
		i++;
	}
	if (args[2])
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		set_exit_status(1);
		return ;
	}
	status = ft_atoi(args[1]) % 256;
	exit(status);
}
