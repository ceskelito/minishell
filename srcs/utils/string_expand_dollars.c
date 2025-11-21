/* ************************************************************************* */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_expand_dollars.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rceschel <rceschel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 17:15:36 by rceschel          #+#    #+#             */
/*   Updated: 2025/11/19 16:40:38 by rceschel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ezgalloc.h"
#include "ft_lib.h"
#include "minishell.h"

/**
 * count_char - Count occurrences of a character in a string
 *
 * @s: String to search in
 * @c: Character to count
 *
 * Return: Number of times character c appears in string s
 */
static int	count_char(char *s, char c)
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
 * fill_chunks - Fill array with string chunks split around delimiter
 *
 * @splitted: Pre-allocated array to fill with chunks
 * @str: String to split (chunks around '$' character)
 *
 * Splits the string into alternating chunks: text before '$' and 
 * variable expressions starting with '$'. Each variable chunk includes
 * the '$' character and the variable name (alphanumeric + '_') or '?'.
 * All chunks are added to the garbage collector.
 */
void	fill_chunks(char **splitted, char *str)
{
	int		i;
	int		len;
	int		n;

	i = 0;
	n = 0;
	while (len = 0, str[i])
	{
		while (str[i + len] && str[i + len] != '$')
			len++;
		if (len == 0 && str[i] == '$')
		{
			len = 1;
			if (str[i + len] == '?')
				len++;
			else
				while (str[i + len] && (ft_isalnum(str[i + len]) || str[i + len] == '_'))
					len++;
		}
		splitted[n++] = ft_substr(str, i, len);
		i += len;
	}
	splitted[n] = NULL;
}

/**
 * ft_split_in_chunks - Split string into chunks around delimiter
 *
 * @str: String to split
 * @delimiter: Character to split around (typically '$')
 *
 * Splits the input string into chunks, preserving the delimiter as part
 * of the following chunk. Used to separate variable expressions from text.
 *
 * Return: NULL-terminated array of string chunks
 */
static char	**ft_split_in_chunks(char *str, char delimiter)
{
	int		delimiter_count;
	char	**splitted;

	delimiter_count = count_char(str, delimiter);
	if (!delimiter_count)
	{
		splitted = ft_calloc(2, sizeof(char *));
		splitted[0] = ft_strdup(str);
	}
	else
	{
		splitted = ft_calloc(delimiter_count * 2 + 1, sizeof(char *));
		fill_chunks(splitted, str);
	}
	return (splitted);
}



/**
 * expand_variable - Expand a single variable chunk
 *
 * @chunk: String starting with '$' followed by variable name or '?'
 *
 * Return: Newly allocated string with the expanded value
 */
static char	*expand_variable(char *chunk)
{
	char	*temp;
	char	*result;

	if (chunk[1] == '?')
	{
		result = ft_itoa(get_exit_status());
		return (result);
	}
	temp = ft_getenv(&(chunk[1]));
	if (temp)
		result = ft_strdup(temp);
	else
		result = ft_strdup("");
	return (result);
}

/**
 * process_chunks - Process and expand all chunks
 *
 * @splitted: Array of string chunks (some starting with '$')
 *
 * Expands variables in chunks starting with '$' and calculates total length.
 * Return: Total length of all expanded chunks
 */
static int	process_chunks(char **splitted)
{
	int		i;
	int		total_len;
	char	*temp;

	i = 0;
	total_len = 0;
	while (splitted[i])
	{
		if (splitted[i][0] == '$' && splitted[i][1])
		{
			temp = expand_variable(splitted[i]);
			splitted[i] = temp;
		}
		total_len += ft_strlen(splitted[i]);
		i++;
	}
	return (total_len);
}

/**
 * join_chunks - Join all chunks into a single string
 *
 * @splitted: Array of string chunks to join
 * @total_len: Total length of the result string
 *
 * Return: Newly allocated string containing all chunks concatenated
 */
static char	*join_chunks(char **splitted, int total_len)
{
	char	*result;
	int		i;

	result = ft_calloc(total_len + 1, sizeof(char));
	i = 0;
	while (splitted[i])
	{
		ft_strlcat(result, splitted[i], total_len + 1);
		i++;
	}
	return (result);
}

/**
 * string_expand_dollars - Expand all dollar variables in a string
 *
 * @str: Input string that may contain $VAR or $? expansions
 * @collapse_spaces: Unused parameter (kept for compatibility)
 *
 * Splits the string into chunks around '$' characters, expands variables,
 * and joins everything back together. Variables are expanded to their
 * environment values or empty string if not found. $? expands to exit status.
 *
 * Return: Newly allocated string with all variables expanded
 */
char	*string_expand_dollars(char *str, bool collapse_spaces)
{
	int		new_len;
	char	**splitted;
	char	*result;

	(void)collapse_spaces;
	if (!ft_strchr(str, '$'))
		return (ft_strdup(str));
	splitted = ft_split_in_chunks(str, '$');
	new_len = process_chunks(splitted);
	result = join_chunks(splitted, new_len);
	free(splitted);
	return (result);
}
