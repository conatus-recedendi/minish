/*
**	cat.c
**
*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#define BUF_SIZE 2

int		main(int argc, char *argv[])
{
	int	i;
	int	fd;
	char	buf[BUF_SIZE];
	int	size;

	buf[1] = 0;
	i = 1;
	if (argc == 1)
	{
		while (read(STDIN_FILENO, buf, BUF_SIZE - 1) > 0)
		{
			printf("%s", buf);
		}
	}
	else
	{
		while (i < argc)
		{
			if ((fd = open(argv[i], O_RDWR)) != -1)
			{
				while ((size = read(fd, buf, BUF_SIZE - 1)) > 0)
				{
					printf("%c", buf[0]);
				}
			}
			else
			{
				dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", argv[0], argv[i]);
				exit(2);
			}
			i++;
		}
	}
	printf("\n");
	exit(0);
}