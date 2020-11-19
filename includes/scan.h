/*
**	scan.h
**
*/

#ifndef SCAN_H
# define SCAN_H

# include <stdlib.h>

# include "unistd.h"
# include "string.h"

# define SCAN_LEN_MAX 1024
# define SCAN_TEMP_LEN_MAX 256

char		*get_line(int fd);
char		*line_append(const char *des, const char *append);

#endif