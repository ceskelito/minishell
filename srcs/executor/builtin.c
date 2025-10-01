#include "executor.h"

void change_directory(char *dirname)
{
	// Need to manage the "too many arguments" case.
	// idk how to do it before the end of tokenizer
	//----
	// is also needed to manage the case in wich HOME env is not set
	if (!dirname)
	{
		chdir(getenv("HOME"));
		return;
	}
	if (strcmp(dirname, "") == 0)
		return;	
	if (chdir(dirname) != 0)
		perror(ft_strjoin("cd: ", dirname));
}
