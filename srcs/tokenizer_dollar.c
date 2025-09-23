#include "minishell.h"

static char	*expand_exit_status(char *result, int *i)
{
	char	*temp;

	(*i)++;
	temp = ft_strjoin(result, "$?");
	free(result);
	return (temp);
}

static char	*expand_variable(char *input, int *i, char *result, int start)
{
	int		var_len;
	char	*var_part;
	char	*temp;

	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_len = *i - start;
	var_part = ft_substr(input, start, var_len);
	temp = ft_strjoin(result, var_part);
	free(result);
	free(var_part);
	return (temp);
}

void	handle_dollar_sign(char *input, int *i, char **result)
{
	int		start;

	start = *i;
	(*i)++;
	if (input[*i] == '?')
	{
		(*result) = expand_exit_status(*result, i);
		return ;
	}
	if (ft_isalpha(input[*i]) || input[*i] == '_')
	{
		*result = expand_variable(input, i, *result, start);
		return ;
	}
	*result = ft_strjoin_char(*result, '$');
}
