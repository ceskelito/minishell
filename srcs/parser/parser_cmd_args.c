/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:32:28 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/16 19:32:33 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ezgalloc.h"

int	token_count_args(t_token *token)
{
	int		count;

	count = 0;
	while (token && (token->type & WORD))
	{
		count++;
		token = token->next;
	}
	return (count);
}

static int	copy_old_args(char **new_args, char **old_args, int old_count)
{
	int	i;

	i = 0;
	while (i < old_count)
	{
		new_args[i] = old_args[i];
		i++;
	}
	return (i);
}

static int	add_new_args(char **new_args, t_token *token, int start, int cnt)
{
	int	i;

	i = start;
	while (i < start + cnt)
	{
		new_args[i] = ft_strdup(token->value);
		if (!new_args[i])
			return (-1);
		ezg_add(COMMAND, new_args[i]);
		token = token->next;
		i++;
	}
	new_args[i] = NULL;
	return (cnt);
}

static int	append_args(t_cmd *cmd, t_token *token, int new_count)
{
	int		old_count;
	char	**new_args;
	int		i;

	old_count = 0;
	while (cmd->args[old_count])
		old_count++;
	new_args = ezg_calloc(COMMAND, old_count + new_count + 1,
			sizeof(char *));
	if (!new_args)
		return (-1);
	i = copy_old_args(new_args, cmd->args, old_count);
	if (add_new_args(new_args, token, i, new_count) < 0)
		return (-1);
	cmd->args = new_args;
	return (new_count);
}

int	set_cmd_args(t_cmd *cmd, t_token *token)
{
	int		new_count;

	new_count = token_count_args(token);
	if (cmd->args)
		return (append_args(cmd, token, new_count));
	cmd->args = ezg_calloc(COMMAND, new_count + 1, sizeof(char *));
	if (!cmd->args)
		return (-1);
	if (add_new_args(cmd->args, token, 0, new_count) < 0)
		return (-1);
	return (new_count);
}
