/*
**	prompt.c
**
*/

#include "prompt.h"

int			prompt(void)
{
	char	*prompt;
	char	*temp;

	prompt = malloc(sizeof(char) * MAX_LEN_PROMPT);
	temp = getcwd(prompt, MAX_LEN_PROMPT);
	if (temp == NULL)
	{
		//error causes. prompt cannot print.
		free(prompt);
		return(ERROR_PROMPT_PRINT);
	}
	print_str(STDOUT_FILENO, temp);
	print_str(STDOUT_FILENO, "$ ");
	return (SUCCESS_PROMPT);
}