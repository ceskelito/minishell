/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand_dollars.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:15:36 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/21 14:00:00 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"

/**
 * count_delimiter - Count occurrences of a specific character in a string
 * @s: The string to search
 * @c: The character to count
 *
 * Return: Number of times character c appears in string s
 */
static int	count_delimiter(char *s, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c)
			count++;
		i++;
	}
	return (count);
}

/**
 * get_chunk_len - Calculate length of next chunk (text or variable)
 * @str: String starting at current position
 * @i: Current position in string
 *
 * Return: Length of the chunk (either up to next $ or variable name length)
 */
static int	get_chunk_len(char *str, int i)
{
	int		len;

	len = 0;
	if (str[i] != '$')
	{
		while (str[i + len] && str[i + len] != '$')
			len++;
	}
	else
	{
		len = 1;
		if (str[i + len] == '?')
			len++;
		else
			while (str[i + len] && (ft_isalnum(str[i + len])
					|| str[i + len] == '_'))
				len++;
	}
	return (len);
}

/**
 * fill_chunks - Split string into chunks separated by $ delimiter
 * @splitted: Array to store the chunks
 * @str: String to split
 *
 * Splits the string into chunks where each chunk is either text before $
 * or a variable reference starting with $
 */
static void	fill_chunks(char **splitted, char *str)
{
	int		i;
	int		len;
	int		n;

	i = 0;
	n = 0;
	while (str[i])
	{
		len = get_chunk_len(str, i);
		splitted[n++] = ft_substr(str, i, len);
		i += len;
	}
	splitted[n] = NULL;
}

/**
 * split_by_dollar - Split string into chunks at $ boundaries
 * @str: String to split
 *
 * Return: NULL-terminated array of string chunks
 */
static char	**split_by_dollar(char *str)
{
	int		count;
	char	**splitted;

	count = count_delimiter(str, '$');
	if (!count)
	{
		splitted = ft_calloc(2, sizeof(char *));
		splitted[0] = ft_strdup(str);
	}
	else
	{
		splitted = ft_calloc(count * 2 + 1, sizeof(char *));
		fill_chunks(splitted, str);
	}
	return (splitted);
}

/**
 * handle_space_sequence - Process a sequence of spaces for word splitting
 * @s: Input string pointer to update
 * @d: Output string pointer to update
 * @leading: Whether this is at the start of string
 *
 * Converts multiple consecutive spaces to single space marker.
 * Preserves one space at start/end if present.
 */
static void	handle_space_sequence(char **s, char **d, bool leading)
{
	while (ft_isspace(**s))
		(*s)++;
	if (!leading || **s)
		*((*d)++) = ' ';
}

/**
 * apply_word_split - Apply bash word splitting to expanded variable
 * @str: String pointer to process and replace
 *
 * When word splitting is enabled, collapses multiple spaces to single spaces
 * while preserving markers for leading/trailing spaces. This prepares the
 * string for later splitting into multiple tokens.
 */
static void	apply_word_split(char **str)
{
	char	*src;
	char	*dst;
	char	*new;

	src = *str;
	new = ft_calloc(ft_strlen(src) + 1, sizeof(char));
	if (!new)
	{
		*str = NULL;
		return ;
	}
	dst = new;
	if (ft_isspace(*src))
		handle_space_sequence(&src, &dst, true);
	while (*src)
	{
		if (ft_isspace(*src))
			handle_space_sequence(&src, &dst, false);
		else
			*dst++ = *src++;
	}
	*dst = '\0';
	free(*str);
	*str = new;
}

/**
 * expand_exit_status - Expand $? to current exit status value
 *
 * Return: String representation of exit status (allocated)
 */
static char	*expand_exit_status(void)
{
	return (ft_itoa(get_exit_status()));
}

/**
 * expand_variable - Expand environment variable reference
 * @var_name: Name of the variable (without $)
 * @word_split: Whether to apply word splitting
 *
 * Return: Expanded variable value (allocated), empty string if not found
 */
static char	*expand_variable(char *var_name, bool word_split)
{
	char	*value;
	char	*result;

	value = ft_getenv(var_name);
	if (value)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	if (word_split && result)
		apply_word_split(&result);
	return (result);
}

/**
 * expand_chunk - Expand a single chunk if it's a variable reference
 * @chunk: Pointer to chunk string to expand
 * @word_split: Whether to apply word splitting to variables
 *
 * If chunk starts with $, replaces it with expanded value.
 * Otherwise leaves chunk unchanged.
 */
static void	expand_chunk(char **chunk, bool word_split)
{
	char	*expanded;

	if (!*chunk || (*chunk)[0] != '$' || !(*chunk)[1])
		return ;
	if ((*chunk)[1] == '?')
		expanded = expand_exit_status();
	else
		expanded = expand_variable(&(*chunk)[1], word_split);
	free(*chunk);
	*chunk = expanded;
}

/**
 * calculate_length - Calculate total length of all chunks
 * @chunks: NULL-terminated array of strings
 *
 * Return: Sum of lengths of all chunks
 */
static int	calculate_length(char **chunks)
{
	int		total;
	int		i;

	total = 0;
	i = 0;
	while (chunks[i])
		total += ft_strlen(chunks[i++]);
	return (total);
}

/**
 * join_chunks - Concatenate all chunks into single string
 * @chunks: NULL-terminated array of string chunks
 * @total_len: Total length needed for result
 *
 * Return: Newly allocated concatenated string
 */
static char	*join_chunks(char **chunks, int total_len)
{
	char	*result;
	int		i;

	result = ft_calloc(total_len + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (chunks[i])
		ft_strlcat(result, chunks[i++], total_len + 1);
	return (result);
}

/**
 * string_expand_dollars - Expand dollar variables in a string
 * @str: Input string potentially containing $VAR or $? references
 * @word_split: If true, apply word splitting to expanded variables
 *
 * Processes string to expand all $VAR references to their values and
 * $? to the last exit status. When word_split is true, applies bash-style
 * word splitting: collapses multiple spaces to single spaces and marks
 * boundaries for later tokenization while preserving leading/trailing space.
 *
 * Return: Newly allocated string with expansions applied, or NULL on error
 */
char	*string_expand_dollars(char *str, bool word_split)
{
	char __attribute__((cleanup(clean_array)))	**chunks;
	int		len;
	int		i;

	chunks = NULL;
	if (!ft_strchr(str, '$'))
		return (ft_strdup(str));
	chunks = split_by_dollar(str);
	if (!chunks)
		return (NULL);
	i = 0;
	while (chunks[i])
	{
		expand_chunk(&chunks[i], word_split);
		i++;
	}
	len = calculate_length(chunks);
	return (join_chunks(chunks, len));
}
