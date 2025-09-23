#include "minishell.h"

int main(void)
{
	char	*prompt = "rceschel >	\0";
	char	*input;
	for(int i = 0; i < 3; i++)
	{
		input = readline(prompt);
		tokenize_input(input);
		free(input);
		free(prompt);
	}
}
