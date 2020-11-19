/*
**	exec.c
**
*/

#include "exec.h"

#include <unistd.h>
#include <fcntl.h>

#define EXIT_COMMAND 1
#define PATH_BUFFER_SIZE 1024

extern char	**environ;
extern int	quit;
extern int	last_status;
char	**make_argu(t_node *parser_head)
{
	t_node	*node;
	int		cnt;
	char	**ret;
	int		i;
	int		len;

	cnt = 0;
	node = parser_head;
	while (node)
	{
		if (node->desc >= 1 && node->desc <= 4)
		{
			cnt++;
		}
		else
			break ;
		node = node->next;
	}
	ret = malloc(sizeof(char *) * (cnt + 1));
	ret[cnt] = NULL;
	i = 0;
	node = parser_head;
	while (i < cnt)
	{
		ret[i] = strdup(node->word);
		node = node->next;
		i++;
	}
	return (ret);
}

char		*exec(t_node *parser_head)
{
	int		i;
	pid_t	pid;
	int		pid_status;
	t_node	*next;	
	int		last;
	int		error;

	i = 0;
	error = 0;
	last = 0;
	next = check_pipe(parser_head, &last, &error);
	if (error == 1)
		return (next->word);
	if (next == NULL)
		return (NULL);
	//check_redir();
	char path[100] = "./srcs/bin/";
	strcat(path, parser_head->word);
	char **argu = make_argu(parser_head);
	int fd = open("test.txt", O_WRONLY |O_CREAT |O_APPEND, 0666);
	if (!strcmp(parser_head->word, "path"))
	{
		dprintf(fd, "[cmd]path\n");
		if (argu[1] == NULL)
		{
			dprintf(STDOUT_FILENO, "%s\n", getenv("PATH"));
		}
		else
		{
			setenv("PATH", argu[1], 1);
		}
		
		if (last == 1)
			return (NULL);
		exit(EXIT_SUCCESS);
	}
	else if (!strcmp(parser_head->word, "cd"))
	{
		dprintf(fd, "[cmd]cd\n");
		char cdpath[PATH_BUFFER_SIZE];

		if (argu[1] == NULL)
		{
			chdir(getenv("HOME"));
		}
		else
		{
			getcwd(cdpath, sizeof(cdpath));
			strcat(cdpath, "/");
			strcat(cdpath, argu[1]);
			chdir(cdpath);
		}
		if (last == 1)
			return (NULL);
		exit(EXIT_SUCCESS);
	}
	else if (!strcmp(parser_head->word, "status"))
	{
		dprintf(fd, "[cmd]status\n");
		dprintf(STDOUT_FILENO, "%d\n", last_status);
		if (last == 1)
			return (NULL);
		exit(EXIT_SUCCESS);
	}
	else if (!strcmp(parser_head->word, "quit"))
	{
		dprintf(fd, "[cmd]quit\n");
		quit = 1;
		if (last == 1)
			return (NULL);
		exit(EXIT_SUCCESS);
	}
	else 
	{
		pid = fork();
		if (pid == 0)
		{
			if (execve(path, argu, NULL) == -1)
			{
				dprintf(STDERR_FILENO, "minish: command not found: %s\n", parser_head->word);
				//ft_cmd_not_found(parser_head->word);
				dprintf(fd, "one\n");
				/*
				if (last == 1)
				{
					return (parser_head->word);
				}
				*/
				exit(EXIT_FAILURE);
			}
			exit(0);
		}
		dprintf(fd, "two\n");
		int	status;
		waitpid(pid, &status, 0);
		if (256 * 1 == status && last == 1)
			return (parser_head->word);
		if (256 * 1 == status)
		{
			exit(EXIT_FAILURE);
		}
		dprintf(fd, "four\n");
		if (last == 1)
			return (NULL);
		exit(EXIT_SUCCESS);
	}
	//dprintf(fd, "?\n");
	return (NULL);
}