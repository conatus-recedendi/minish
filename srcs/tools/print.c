/*
**	print.c
**
*/

#include "print.h"

/*
static void	itoa(int n, char *buf)
{

}
*/

int			print_str(int fd, const char *str)
{
	int		size;

	size = strlen(str);
	return (write(fd, str, size));
}

/*
int			print_int(int fd, const int num)
{
	int		size;
	char	buf[30];

	itoa(num, buf);
	return(print_str(fd, buf));
}
*/