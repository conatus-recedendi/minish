/*
**	prompt.h
**
*/

#ifndef PROMPT_H
# define PROMPT_H

# include <stdlib.h>
# include <unistd.h>
# include "print.h"

# define MAX_LEN_PROMPT 1024
# define ERROR_PROMPT_PRINT 1
# define SUCCESS_PROMPT 0

int		prompt(void);
#endif