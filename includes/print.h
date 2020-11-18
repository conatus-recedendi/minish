/*
**	print.h
**
*/

#ifndef PRINT_H
#define PRINT_H

# include <unistd.h>
# include <string.h>
# include <stdlib.h>

int		print_str(int fd, const char *str);
int		print_int(int fd, const int num);
#endif
