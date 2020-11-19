/*
**	minish.c
**
*/

#include "minish.h"

void		sig_int_handler(int sig_num)
{
	sig_num = 0;
	write(STDOUT_FILENO, "\n", 1);
	prompt();
	flag = 1;
	return ;
}

void		init_parser(t_parser *p)
{
	t_node	*node;

	while (p->head)
	{
		node = p->head;
		p->head = p->head->next;
		free(node);
	}
	p->head = NULL;
	p->tail = NULL;
}

int			main(void)
{
	char	*line;
	t_parser	split;
	int		exit_status;
	char	*error_cmd;

	strcat(path, getenv("PWD"));
	strcat(path, "/srcs/bin/");
	signal(SIGINT, sig_int_handler);
	exit_status = 0;
	std_fd[STDIN_FILENO] = dup(STDIN_FILENO);
	std_fd[STDOUT_FILENO] = dup(STDOUT_FILENO);
	while (1)
	{
		wildcard_error = 0;
		if (!flag)
		{
			if (prompt())
				return (1);
		}
		else
			flag = 0;
		init_parser(&split);
		line = lex();
		if (line == NULL)
			break ;
		split = str_split(line, " \t\n");
		if (wildcard_error)
			continue ;
		if ((error_cmd = exec(split.head)) != NULL)
		{
			dup2(std_fd[STDOUT_FILENO], STDOUT_FILENO);
			last_status = 1;
		}
		else
			last_status = 0;
		dup2(std_fd[STDIN_FILENO], STDIN_FILENO);
		dup2(std_fd[STDOUT_FILENO], STDOUT_FILENO);
		if (quit)
			break ;
	}
}


