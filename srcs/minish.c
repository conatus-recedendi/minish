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

int			main(void)
{
	char	*line;
	t_parser	split;
	int		*desc;
	int		exit_status;
	char	*error_cmd;

	exit_status = 0;
	std_fd[STDIN_FILENO] = dup(STDIN_FILENO);
	std_fd[STDOUT_FILENO] = dup(STDOUT_FILENO);
	while (1)
	{
		if (prompt())
			return (1);
		// print current directory path in a prompt.
		// It depend on the current environment value.
		line = lex();
		if (line == NULL)
			break ;
		split = str_split(line, " \t\n");
		t_node *node = split.head;
		while (node)
		{
			printf("[%d:%s]-->",node->desc, node->word);		
			node = node->next;
		}
		printf("\n");
		// line에서 space, tab, newline을 기준으로 단어를 제거해준다.
		// 단, "", ''으로 묶여있는 건 하나의 문자열로 반환한다.
		// 또한, || && | &와 같은 기호는 자리를 하나 차지한다.
		// split은 위의 규칙대로 자른 각각의 문자열, 기호들이 놓여있는 배열이다.
		// des는 해당 배열의 각각의 위치가 어떤 것을 의미하는지 나타낸다. 
		// 예를 들어, des[0] = 1이면, split[0]의 문자열은 cmd임을 나타낸다.
		// 만약에, *(zero or more arbitrarty character), ?(one arbitrary character)
		// 를 구현하고자하면, 여기서 모두 처리해주어야 함.
		if ((error_cmd = exec(split.head)) != NULL)
		{
			dup2(std_fd[STDOUT_FILENO], STDOUT_FILENO);
			printf("minish: command not found: %s\n", error_cmd);
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


