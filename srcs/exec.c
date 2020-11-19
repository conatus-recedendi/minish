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

// 오류없을 시 0, 오류가 있을 시 1
int		check_redir(t_node *parser_head, int *last)
{
	t_node	*temp;
	int		fd;
	int		pid;
	int		status;

	temp = parser_head;
	while (temp && temp->desc != 11)
	{
		if (temp->desc == 6)
		{
			// >
			fd = open(temp->word, O_WRONLY | O_CREAT, 0666);
			if (fd < 0)
			{
				dprintf(STDERR_FILENO, "minish: no such file or directory: %s\n", temp->word);
				if (*last)
					return (1);
				exit(EXIT_FAILURE);
			}
			pid = fork();
			if (pid == 0)
			{
				dup2(fd, STDOUT_FILENO);
				*last = 0;
				return (0);
			}
			waitpid(pid, NULL, 0);
			close(fd);
			if (status == 256 * 1 || *last)
				return (1);
		}
		else if (temp->desc == 8)
		{
			// <
			fd = open(temp->word, O_RDONLY, 0666);
			if (fd < 0)
			{
				dprintf(STDERR_FILENO, "minish: no such file or directory: %s\n", temp->word);
				if (*last)
					return (1);
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);

		}
		else if (temp->desc == 10)
		{
			// >>
			fd = open(temp->word, O_WRONLY | O_CREAT | O_APPEND, 0666);
			if (fd < 0)
			{
				dprintf(STDERR_FILENO, "minish: no such file or directory: %s\n", temp->word);
				if (*last)
					return (1);
				exit(EXIT_FAILURE);
			}
			pid = fork();
			if (pid == 0)
			{
				dup2(fd, STDOUT_FILENO);
				*last = 0;
				return (0);
			}
			waitpid(pid, &status, 0);
			close(fd);
			if (status == 256 * 1 || *last)
				return (1);
		}
		temp = temp->next;
	}
	return (0);
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
	if (check_redir(parser_head, &last))
	{
		if (last == 1)
			return (NULL);
		exit(EXIT_FAILURE);
	}
	char path[100] = "./srcs/bin/";
	strcat(path, parser_head->word);
	char **argu = make_argu(parser_head);
	if (!strcmp(parser_head->word, "path"))
	{
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
	else if (!strcmp(parser_head->word, "jobs"))
	{

	}
	else if (!strcmp(parser_head->word, "kjob"))
	{
		
	}
	else if (!strcmp(parser_head->word, "cd"))
	{
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
			if (chdir(cdpath) == -1)
			{
				dprintf(STDERR_FILENO, "%s: no such file or directory: %s\n", "cd", cdpath);
				if (last)
				{
					return (parser_head->word);
				}
				exit(EXIT_FAILURE);
			}
		}
		if (last == 1)
			return (NULL);
		exit(EXIT_SUCCESS);
	}
	else if (!strcmp(parser_head->word, "status"))
	{
		dprintf(STDOUT_FILENO, "%d\n", last_status);
		if (last == 1)
			return (NULL);
		exit(EXIT_SUCCESS);
	}
	else if (!strcmp(parser_head->word, "quit"))
	{
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
		int	status;
		waitpid(pid, &status, 0);
		if (256 * 1 == status && last == 1)
		{
			return (parser_head->word);
		}
		if (256 * 1 == status)
		{
			exit(EXIT_FAILURE);
		}
		if (last == 1)
			return (NULL);
		exit(EXIT_SUCCESS);
	}
	//dprintf(fd, "?\n");
	return (NULL);
}