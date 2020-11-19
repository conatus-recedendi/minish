/*
**	sleep.c
**
*/

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void	sigint(int sig)
{
	exit (1);
}
int		main(int argc, char *argv[])
{
	int	num;
	unsigned int	left;

	signal(SIGINT, sigint);
	if (argc == 2)
	{
		num = atoi(argv[1]);
		if (sleep(num))
		{
			exit(1);
		}
		exit(0);
	}
	dprintf(STDERR_FILENO, "usage: sleep seconds\n");
	exit(1);
}