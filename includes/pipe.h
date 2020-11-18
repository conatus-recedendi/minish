/*
**	pipe.h
**
*/

#ifndef PIPE_H
#define PIPE_H

# include "exec.h"
# include "split.h"
# include "stdlib.h"
# include "string.h"

t_node		*check_pipe(t_node *head, int *last, int *error);

#endif