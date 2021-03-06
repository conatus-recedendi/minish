/*
**	pipe.c
**
*/

#include "pipe.h"

t_node		*check_pipe(t_node *head, int *last, int *error)
{
	int	i;
	int	fd[2];
	int	state;
	pid_t pid;
	t_node	*temp;
	char	*ret;

	i = 0;
	*last = 0;
	temp = head;
	while (temp)
	{
		if (temp->desc == 11)
		{
			pipe(fd);
			temp = temp->next;
			pid = fork();
			if (pid == 0)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				return (head);
			}
			else
			{
				dup2(fd[0], STDIN_FILENO);
				close(fd[1]);
				waitpid(pid, &state, 0);
				if (state == 256 * 1)
				{
					*error = 1;
					return (head);
				}
				if ((ret = exec(temp)) != NULL)
				{
					*error = 1;
					return (temp);
				}
				close(fd[1]);
				close(fd[0]);
				return (EXIT_SUCCESS);
			}
			exit(EXIT_SUCCESS);
		}
		temp = temp->next;
	}
	*last = 1;
	return (head);
}