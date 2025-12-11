/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo-cd-pwd-exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:42:41 by rceschel          #+#    #+#             */
/*   Updated: 2025/12/11 16:27:01 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char __attribute__	((cleanup(clean_char)))	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		perror("minishell: pwd");
	else
		ft_printf("%s\n", cwd);
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
	int	i;

	ft_printf("exit\n");
	if (!args || !args[1])
	{
		ezg_cleanup();
		exit(get_exit_status());
	}
	if (args[2])
		return (print_error("exit", "too many arguments"), set_exit_status(1));
	i = 0;
	while (args[1][i])
	{
		if (!ft_isdigit(args[1][i]) &&
			!(i == 0 && (args[1][i] == '+' || args[1][i] == '-')))
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: exit: %s: numeric argument required\n", args[1]);
			ezg_cleanup();
			exit(255);
		}
		i++;
	}
	ezg_cleanup();
	exit(ft_atoi(args[1]) % 256);
}
