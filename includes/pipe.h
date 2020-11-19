/*
**	pipe.h
**
*/

#ifndef PIPE_H
# define PIPE_H

# include <stdlib.h>
# include <string.h>

# include <unistd.h>
# include <fcntl.h>
# include "exec.h"
# include "split.h"

t_node		*check_pipe(t_node *head, int *last, int *error);

#endif