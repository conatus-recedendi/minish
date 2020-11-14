/*
**	lex.c
**
**	
*/

#include "scan.h"
#include "lex.h"

int		isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

int		check_newline(char *line, int *index, int bf)
{
	int	i;
	int	flag;
	int	last_flag;
	int	first_flag;

	i = *index;
	flag = (bf == -1) ? 0 : bf;
	last_flag = 0;
	first_flag = (bf == -1) ? 1 : 0;
	while (line[i])
	{
		if (!flag)
		{
			if (line[i] == '\"')
				flag = '\"';
			else if (line[i] == '\'')
				flag = '\'';
		}
		else if (line[i] == flag)
			flag = 0;
		if (!isspace(line[i]) && !first_flag)
			first_flag = 1;
		if (first_flag == 1 && i > 0)
		{
			if (line[i - 1] == '&' && line[i] == '&')
				last_flag = 1; // and
			else if (line[i - 1] == '|' && line[i] == '|')
				last_flag = 2; // or
			else if (isspace(line[i - 1]) && line[i] == '|')
				last_flag = 3; // pipe
			else if (!isspace(line[i]))
				last_flag = 0;
		}
		i++;
	}
	*index = i;
	if (flag)
	{
		if (flag == '\"')
			print_str(STDOUT_FILENO, "dquote> ");
		else if (flag == '\'')
			print_str(STDOUT_FILENO, "quote> ");
		// printf("> "); ( or \
		// printf("dquote> "); "
		// printf("quote> "); '
		// printf("cmdand> "); &&
		// printf("cmdor> "); ||
		// printf("pipe> "); |
		return (flag);
	}
	else if (last_flag)
	{
		if (last_flag == 1)
			print_str(STDOUT_FILENO, "cmdand> ");
		else if (last_flag == 2)
			print_str(STDOUT_FILENO, "cmdor> ");
		else if (last_flag == 3)
			print_str(STDOUT_FILENO, "pipe> ");
		return (-1);
	}
	return (flag);
}

char		*lex(void)
{
	char	*line;
	char	*cmd = "";
	int		fd;
	int		index;
	int		flag;

	if ((cmd = get_line(STDIN_FILENO)) == NULL)
	{
		return (NULL);
		// cannot read by some reasons.
	}
	fd = 1;
	index = 0;
	flag = 0;
	while ((flag = check_newline(cmd, &index, flag)))
	{
		if ((line = get_line(STDIN_FILENO)) == NULL)
		{
			//cannot read by some reasons.
			return (NULL);
		}
		cmd = line_append(cmd, line);
	}
	return (cmd);
}
