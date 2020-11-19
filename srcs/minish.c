/*
**	minish.c
**
*/

#include "prompt.h"
#include "exec.h"
#include "lex.h"
#include "pipe.h"
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int			flag;

void		sig_int_handler(int sig_num)
{
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
	int		*desc;
	int		exit_status;
	char	*error_cmd;
	int		fd[2];

//	sigaction(SIGINT, &(struct sigaction){ .sa_handler = sig_int_handler }, NULL);
	signal(SIGINT, sig_int_handler);
	exit_status = 0;
	std_fd[STDIN_FILENO] = dup(STDIN_FILENO);
	std_fd[STDOUT_FILENO] = dup(STDOUT_FILENO);
	while (1)
	{
		quit = 0;
		if (!flag)
		{
			if (prompt())
				return (1);
		}
		else
			flag = 0;
		init_parser(&split);
		// print current directory path in a prompt.
		// It depend on the current environment value.
		line = lex();
		if (line == NULL)
			break ;
		split = str_split(line, " \t\n");
		if (quit)
			break ;
		t_node *node = split.head;
		if ((error_cmd = exec(split.head)) != NULL)
		{
			dup2(std_fd[STDOUT_FILENO], STDOUT_FILENO);
			//dprintf(STDERR_FILENO, "minish: command not found: %s\n", error_cmd);
			last_status = 1;
		}
		else
			last_status = 0;
		// restore stdin, stdout
		dup2(std_fd[STDIN_FILENO], STDIN_FILENO);
		dup2(std_fd[STDOUT_FILENO], STDOUT_FILENO);
		if (quit)
			break ;
		/*
		node = split.head;
		while ((node = check_pipe(node)) != NULL)
		{
			exec(split.head);
		}
		*/
		//exec(check_pipe(split.head));
		// execute the command.
		// This doesn't implement exact shell syntax.
		// So, I don't make the general rules.
	}
}


