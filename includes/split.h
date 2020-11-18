/*
**	split.h
**
*/

#ifndef SPLIT_H
# define SPLIT_H
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct		s_node
{
	char			*word;
	int				desc;
	// 1 : cmd
	// 2 : dquote. but it considered as argu.
	// 3 : quote. but it considered as argu.
	// 4 : argu
	// 5 : >
	// 6 : > redir files
	// 7 : <
	// 8 : < redire files
	// 9 : >>
	// 10 :  >> redir files
	// 11 : |
	// 12 : &&
	// 13 : ||
	// 14 : & (background)
	struct s_node	*next;
}					t_node;

typedef struct		s_parser
{
	t_node			*head;
	t_node			*tail;
}					t_parser;

static	char		*strdup_ptr(char *start, char *end);
static	int	check_set(char c, const char *set);
t_parser		str_split(const char *line, const char *set);



#endif