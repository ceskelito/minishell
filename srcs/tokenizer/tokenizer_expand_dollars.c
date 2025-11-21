/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_expand_dollars.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:06:08 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/19 16:08:14 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * create_word_token - Create a new WORD token with specified value
 *
 * @value: String value for the token
 *
 * Creates a new token of type WORD with expand_dollar=false and
 * cat_to_next=false, as word-split tokens are already expanded.
 *
 * Return: Newly allocated token
 */
static t_token	*create_word_token(char *value)
{
	t_token	*new;

	new = ezg_calloc(TOKEN, sizeof(t_token), 1);
	new->value = value;
	new->type = WORD;
	new->expand_dollar = false;
	new->cat_to_next = false;
	new->collapse_spaces = false;
	return (new);
}

/**
 * process_word - Process a single word and add it to the token chain
 *
 * @word: Word string to process
 * @first: Pointer to first token pointer
 * @prev: Previous token in chain
 *
 * Return: The newly created token
 */
static t_token	*process_word(char *word, t_token **first, t_token *prev)
{
	t_token	*new_token;

	new_token = create_word_token(word);
	ezg_add(TOKEN, word);
	if (!*first)
		*first = new_token;
	else
		prev->next = new_token;
	return (new_token);
}

/**
 * split_into_tokens - Split expanded string into multiple word tokens
 *
 * @expanded: Expanded string that may contain spaces
 * @next_token: Next token in original list (to preserve link)
 *
 * Splits the expanded string by whitespace and creates a new token
 * for each word. Tokens are linked together and the last one points
 * to next_token.
 *
 * Return: Pointer to the first token of the new list, or NULL if no words
 */
static t_token	*split_into_tokens(char *expanded, t_token *next_token)
{
	char	**words;
	t_token	*first;
	t_token	*prev;
	int		i;

	words = ft_split(expanded, ' ');
	if (!words || !words[0])
		return (NULL);
	first = NULL;
	prev = NULL;
	i = 0;
	while (words[i])
	{
		if (words[i][0] != '\0')
			prev = process_word(words[i], &first, prev);
		i++;
	}
	if (prev)
		prev->next = next_token;
	free(words);
	return (first);
}

/**
 * token_cat_to_next - Concatenate current token with next token
 *
 * @token: Pointer to current token pointer
 *
 * This function will not free any memory, because tokens are saved
 * in the TOKEN list of garbage (from libezalloc).
 *
 * Return: 0 on success, -1 on failure
 */
int	token_cat_to_next(t_token **token)
{
	t_token	*curr;
	t_token	*next;
	char	*new_value;

	curr = *token;
	next = curr->next;
	if (!next || !curr->cat_to_next || curr->type != next->type)
		return (-1);
	new_value = ezg_calloc(TOKEN, sizeof(char),
			ft_strlen(curr->value) + ft_strlen(next->value) + 1);
	if (!new_value)
		return (-1);
	ft_sprintf(new_value, "%s%s", curr->value, next->value);
	curr->value = new_value;
	new_value = NULL;
	curr->cat_to_next = next->cat_to_next;
	curr->next = next->next;
	return (0);
}

/**
 * expand_and_split_token - Expand variables and perform word splitting
 *
 * @curr: Pointer to current token pointer
 *
 * Expands dollar variables in the token value. If the token is not quoted
 * (collapse_spaces == false), performs word splitting by creating new tokens
 * for each word in the expanded string.
 *
 * Return: Pointer to the last token created (for list traversal)
 */
static t_token	*expand_and_split_token(t_token **curr)
{
	t_token	*split;
	t_token	*last;
	char	*expanded;

	expanded = string_expand_dollars((*curr)->value, (*curr)->collapse_spaces);
	ezg_add(TOKEN, expanded);
	if (!expanded)
		return (NULL);
	if (!(*curr)->collapse_spaces && ft_strchr(expanded, ' '))
	{
		split = split_into_tokens(expanded, (*curr)->next);
		if (split)
		{
			last = split;
			while (last->next && last->next != (*curr)->next)
				last = last->next;
			*curr = split;
			return (last);
		}
	}
	(*curr)->value = expanded;
	return (*curr);
}

/**
 * concatenate_tokens - Concatenate adjacent tokens with cat_to_next flag
 *
 * @token_list: Pointer to the head of the token list
 *
 * Processes the token list and concatenates adjacent tokens where the
 * cat_to_next flag is set.
 */
static void	concatenate_tokens(t_token **token_list)
{
	t_token	*curr;

	curr = *token_list;
	while (curr)
	{
		if (curr->cat_to_next)
		{
			token_cat_to_next(&curr);
			continue ;
		}
		curr = curr->next;
	}
}

/**
 * token_expand_dollars_and_cat - Expand dollars and perform word splitting
 *
 * @token_list: Pointer to the head of the token list
 *
 * First pass: expands all dollar variables and performs word splitting
 * for unquoted tokens. Second pass: concatenates adjacent tokens with
 * the cat_to_next flag set.
 *
 * Return: 0 on success, -1 on failure
 */
int	token_expand_dollars_and_cat(t_token **token_list)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*result;

	curr = *token_list;
	prev = NULL;
	while (curr)
	{
		if (curr->expand_dollar)
		{
			result = expand_and_split_token(&curr);
			if (!result)
				return (-1);
			if (prev)
				prev->next = curr;
			else
				*token_list = curr;
			curr = result;
		}
		prev = curr;
		curr = curr->next;
	}
	concatenate_tokens(token_list);
	return (0);
}
