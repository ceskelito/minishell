/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FIXED - YOUR ZONE                          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 15:45:00 by fixer             #+#    #+#             */
/*   Updated: 2025/12/12 15:45:00 by fixer            ###   ########.fr       */
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

/*
** ✅ FIX: Исправлено добавление аргументов
** 
** ПРОБЛЕМА БЫЛА:
** set_cmd_args() ПЕРЕЗАПИСЫВАЛ cmd->args каждый раз!
** 
** Пример: echo hi < file bye bye
** 1. Первый вызов: cmd->args = ["echo", "hi", NULL]
** 2. После редиректа второй вызов: cmd->args = ["bye", "bye", NULL]
** 3. ПОТЕРЯЛИ "echo" и "hi"! ❌
** 
** РЕШЕНИЕ:
** Если cmd->args уже существует, ДОБАВЛЯЕМ к нему новые аргументы
*/
int	set_cmd_args(t_cmd *cmd, t_token *token)
{
	int		i;
	int		old_count;
	int		new_count;
	char	**new_args;

	new_count = token_count_args(token);
	
	// ✅ Если уже есть аргументы, добавляем к ним
	if (cmd->args)
	{
		// Считаем старые аргументы
		old_count = 0;
		while (cmd->args[old_count])
			old_count++;
		
		// Создаём новый массив для старых + новых аргументов
		new_args = ezg_calloc(COMMAND, old_count + new_count + 1, sizeof(char *));
		if (!new_args)
			return (-1);
		
		// Копируем старые
		i = 0;
		while (i < old_count)
		{
			new_args[i] = cmd->args[i];
			i++;
		}
		
		// Добавляем новые
		while (i < old_count + new_count)
		{
			new_args[i] = ft_strdup(token->value);
			if (!new_args[i])
				return (-1);
			ezg_add(COMMAND, new_args[i]);
			token = token->next;
			i++;
		}
		new_args[i] = NULL;
		cmd->args = new_args;
		return (new_count);
	}
	
	// ✅ Если args ещё нет, создаём новый массив
	cmd->args = ezg_calloc(COMMAND, new_count + 1, sizeof(char *));
	if (!cmd->args)
		return (-1);
	i = 0;
	while (i < new_count)
	{
		cmd->args[i] = ft_strdup(token->value);
		if (!cmd->args[i])
			return (-1);
		ezg_add(COMMAND, cmd->args[i]);
		token = token->next;
		i++;
	}
	cmd->args[i] = NULL;
	return (new_count);
}

int	go_next_cmd(t_cmd **cmd)
{
	(*cmd)->next = ezg_calloc(COMMAND, 1, sizeof(t_cmd));
	if (!(*cmd)->next)
		return (-1);
	*cmd = (*cmd)->next;
	return (0);
}

void	add_pipe_redir(t_cmd *cmd, t_token_type type)
{
	t_redir	*r;

	r = ezg_calloc(GLOBAL, 1, sizeof(t_redir));
	if (!r)
		return ;
	r->type = PIPE | type;
	r->pipe_fd = -1;
	r->file = NULL;
	add_redir(cmd, r);
}