#include "minishell.h"

static char	*get_exit_status_value(void)
{
	extern int	g_sig_status;

	return (ezg_add(TOKEN, ft_itoa(g_sig_status))); // ✅ ИСПРАВЛЕНО: используем ezg_add
}

static char	*get_variable_name(char *input, int start, int end)
{
	return (ezg_add(TOKEN, ft_substr(input, start, end - start))); // ✅ ezg_add
}

static char	*expand_exit_status(char *result)
{
	char	*exit_val;
	char	*temp;

	exit_val = get_exit_status_value();
	if (!exit_val)
		return (result);
	temp = ezg_add(TOKEN, ft_strjoin(result, exit_val)); // ✅ ezg_add
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
	temp = ezg_add(TOKEN, ft_strjoin(result, var_value)); // ✅ ezg_add
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
		temp = ezg_add(TOKEN, ft_strjoin_char(*result, '$')); // ✅ ezg_add
		*result = temp;
		return ;
	}
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	*result = expand_variable(input, start, *i, *result);
}

/*
 * ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ ИЗ variable_expansion.c:
 */

/*
 * find_var_end - Find end of variable name
 */
static int	find_var_end(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '?')
		return (1);
	
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	
	return (i);
}

/*
 * expand_single_var - Expand a single environment variable
 */
static char	*expand_single_var(char *var_name)
{
	char	*value;

	if (!var_name)
		return (ezg_add(TOKEN, ft_strdup("")));
	
	if (ft_strcmp(var_name, "?") == 0)
		return (ezg_add(TOKEN, ft_itoa(get_exit_status())));
	
	value = getenv(var_name);
	if (!value)
		return (ezg_add(TOKEN, ft_strdup("")));
	
	return (ezg_add(TOKEN, ft_strdup(value)));
}

/*
 * expand_variables - Expand all variables in a string
 * @str: String potentially containing $VAR patterns
 * @expand_flag: Whether to expand variables (false for single quotes)
 * Return: String with variables expanded
 */
char	*expand_variables(char *str, bool expand_flag)
{
	char	*result;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		i;
	int		var_len;

	if (!str || !expand_flag)
		return (ezg_add(TOKEN, ft_strdup(str ? str : "")));

	result = ezg_add(TOKEN, ft_strdup(""));
	i = 0;
	
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			var_len = find_var_end(str + i + 1);
			if (var_len > 0)
			{
				var_name = ezg_add(TOKEN, ft_substr(str, i + 1, var_len));
				var_value = expand_single_var(var_name);
				temp = ezg_add(TOKEN, ft_strjoin(result, var_value));
				result = temp;
				i += var_len + 1;
			}
			else
			{
				temp = ezg_add(TOKEN, ft_strjoin_char(result, '$'));
				result = temp;
				i++;
			}
		}
		else
		{
			temp = ezg_add(TOKEN, ft_strjoin_char(result, str[i]));
			result = temp;
			i++;
		}
	}
	
	return (result);
}

/*
 * expand_token_list - Expand variables in all tokens
 */
void	expand_token_list(t_token *tokens)
{
	t_token	*curr;
	char	*expanded;

	curr = tokens;
	while (curr)
	{
		if (curr->type == WORD && curr->expand_dollar)
		{
			expanded = expand_variables(curr->value, true);
			curr->value = expanded;
		}
		curr = curr->next;
	}
}