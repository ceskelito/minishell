#include "minishell.h"

static char	*get_exit_status_value(void)
{
	extern int	g_sig_status;

	return (ft_itoa(g_sig_status));
}

static char	*get_variable_name(char *input, int start, int end)
{
	return (ft_substr(input, start, end - start));
}

static char	*expand_exit_status(char *result)
{
	char	*exit_val;
	char	*temp;

	exit_val = get_exit_status_value();
	if (!exit_val)
		return (result);
	temp = ft_strjoin(result, exit_val);
	free(result);
	free(exit_val);
	return (temp);
}

static char	*expand_variable(char *input, int start, int end, char *result)
{
	char	*var_name;
	char	*var_value;
	char	*temp;

	var_name = get_variable_name(input, start, end);
	if (!var_name)
		return (result);
	var_value = getenv(var_name);
	if (!var_value)
		var_value = "";
	temp = ft_strjoin(result, var_value);
	free(result);
	free(var_name);
	return (temp);
}

void	handle_dollar_sign(char *input, int *i, char **result)
{
	int		start;
	char	*temp;

	(*i)++;
	if (input[*i] == '?')
	{
		(*i)++;
		*result = expand_exit_status(*result);
		return ;
	}
	if (!ft_isalpha(input[*i]) && input[*i] != '_')
	{
		temp = ft_strjoin_char(*result, '$');
		free(*result);  // ← ДОБАВИЛ: освобождаем старую память
		*result = temp;
		return ;
	}
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	*result = expand_variable(input, start, *i, *result);
}