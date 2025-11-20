/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:41 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/20 16:29:01 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

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
	if (strcmp(dir, "") != 0 && chdir(dir) != 0)
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
				"minishell: exit: %s: numeric argument required\n", args[1]);
			exit(255);
		}
		i++;
	}
	if (args[2])
	{
		ft_dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		return (set_exit_status(1));
	}
	status = ft_atoi(args[1]) % 256;
	exit(status);
}
