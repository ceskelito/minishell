#include "minishell.h"

/*
 * expand_var - Expand a single environment variable
 * @var_name: Name of the variable (without $)
 * Return: Value of variable or empty string if not found
 */
static char	*expand_var(char *var_name)
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
 * find_var_end - Find end of variable name
 * @str: String starting at variable name
 * Return: Length of variable name
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
	int		j;
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
				var_value = expand_var(var_name);
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
 * @tokens: List of tokens to expand
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