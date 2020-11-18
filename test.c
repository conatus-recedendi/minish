#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

int main(void)
{
	pid_t	pid;
	printf("%s\n", getenv("A"));
	pid = fork();

	if (pid != 0)
	{
		//child
		setenv("A", "c", 1);
	}
	else
	{
		//parent
		setenv("A", "b", 1);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	printf("%s\n", getenv("A"));
}