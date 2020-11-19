/*
**	wc.c
**
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUF_SIZE 2

int	main(int argc, char *argv[])
{
    int	sbytes;
	int	sword;
	int	sline;
	int	bytes;
	int	word;
	int	line;
	int	i;
	int	ret;
    char buf[BUF_SIZE];
	int	state;
	int	fd;

	int debug = open("test.txt", O_WRONLY |O_CREAT |O_APPEND, 0666);
	dprintf(debug, "wc start\n");

	sbytes = 0;
	sword = 0;
	sline = 0;
	state = 1;
	i = 1;
	buf[BUF_SIZE - 1] = 0;
    if (argc == 1)
    {
        while ((ret = read(STDIN_FILENO, buf, BUF_SIZE - 1)) > 0)
		{
			//dprintf(debug, "%d", ret);
			//dprintf(debug, "%s", buf);
			sbytes++;
			if (buf[0] == ' ' || buf[0] == '\t')
				state = 1;
			else if (buf[0] == '\n')
			{
				sline++;
				state = 1;
			}
			else
			{
				if (state == 1)
					sword++;
				state = 0;
			}
		}
		dprintf(debug, "hiendddddd\n");
		dprintf(debug, "%8d%8d%8d\n", sline, sword, sbytes);
		printf("%8d%8d%8d\n", sline, sword, sbytes);
    }
    else
    {
		while (i < argc)
		{
			state = 1;
			bytes = 0;
			line = 0;
			word = 0;
			fd = open(argv[i], O_RDONLY, 0666);
			while (read(fd, buf, BUF_SIZE - 1) > 0)
			{
				bytes++;
				if (buf[0] == ' ' || buf[0] == '\t')
					state = 1;
				else if (buf[0] == '\n')
				{
					line++;
					state = 1;
				}
				else
				{
					if (state == 1)
						word++;
					state = 0;
				}
			}
			printf("%8d%8d%8d %s\n", line, word, bytes, argv[i]);
			sbytes += bytes;
			sline += line;
			sword += word;
			i++;
		}
		printf("%8d%8d%8d\n", sline, sword, sbytes);
    }
	dprintf(debug, "wc end\n");
    exit(0);
}