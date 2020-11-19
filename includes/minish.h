/*
**	minish.h
**
*/

#ifndef MINISH_H
# define MINISH_H

# include "split.h"
# include "prompt.h"
# include "exec.h"
# include "lex.h"
# include "pipe.h"

# include <unistd.h>
# include <fcntl.h>
# include <signal.h>

int			flag;
int			last_status;
int			std_fd[2];
int			quit;
int			error;
int			wildcard_error;

#endif