/*
**	exec.c
**
*/

#include "exec.h"

int		exec(char **split, int *desc)
{
	int		i;
	pid_t	pid;
	int		pid_status;

	i = 0;
	if (1)
	{
		pid = fork();
		if (pid == -1)
		{
			// TODO: error
		}
		else if (pid == 0)
		{
			// child process
			if (check_pipe())
			{
				// if pipe exist in front of the present desc
				// if it is, the two process returned 
			}
			exit(1);
		}
		else
		{
			// parent process
			waitpid(pid, &pid_status, 0);
			if (WIFEXITED(pid_status))
			{
				// normal child process return. 
				// child process returend non-0 value.
			}
			else
			{
				// other case
			}
			
		}
	}
}