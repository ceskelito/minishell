/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_word2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 19:31:54 by rodolhop          #+#    #+#             */
/*   Updated: 2025/12/16 19:31:56 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	eof_compute_gap(char *input, int *gap, char *quote);
void	set_token_value(t_token *token, char *input, int gap, bool q);

int	process_word_surrounded(t_token *token, char *input)
{
	int		gap;
	char	quote;

	gap = 1;
	quote = input[0];
	while (input[gap] && input[gap] != quote)
		gap++;
	if (input[gap] != quote)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: unexpected EOF while looking for matching `%c\n",
			quote);
		print_error("syntax error", "unexpected end of file");
		return (-1);
	}
	set_token_value(token, input, gap, true);
	gap++;
	return (gap);
}

int	fill_eof_token(t_token *token, char *input)
{
	int		gap;
	char	quote;

	eof_compute_gap(input, &gap, &quote);
	if (quote != 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: unexpected EOF while looking for matching `%c\n",
			quote);
		print_error("syntax error", "unexpected end of file");
		return (-1);
	}
	token->value = ezg_calloc(TOKEN, gap + 1, sizeof(char));
	if (!token->value)
	{
		perror("minishell:");
		set_exit_status(errno);
		exit_shell(NULL);
	}
	ft_strlcat(token->value, input, gap + 1);
	token->type = WORD;
	token->cat_to_next = false;
	return (gap);
}
