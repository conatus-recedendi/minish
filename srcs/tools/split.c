/*
**	split.c
**
*/

#include "split.h"
#include <dirent.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "minish.h"

extern int			quit;
// end wasn't included.
static	char		*strdup_ptr(char *start, char *end)
{
	char			*ret;
	int				i;

	ret = malloc(sizeof(char) * (end - start + 1));
	if (!ret)
		return (ret);
	ret[end - start] = 0;
	i = 0;
	while (start + i < end)
	{
		ret[i] = *(start + i);
		i++;
	}
	return (ret);
}

static	int	check_set(char c, const char *set)
{
	int		i;

	i = 0;
	while (*(set + i))
	{
		if (c == *(set + i))
			return (1);
		i++;
	}
	return (0);
}

t_parser		str_split(const char *line, const char *set)
{
	t_parser	ret;
	char		*dup;
	char		*pptr;
	char		*word;
	int			i;
	int			special;
	int			is_cmd;
	int			quote;
	int			desc;
	int			end_cmd;
	int			flag;
	int			wildcard;

	// 문자열을 나누는 기준을 정해보자. 
	// 1. dquote, quote가 처음 열린 상태에서 동일한 기호가 나타날 때까지 반복.
	// 2. 각 명령어 줄의 시작은 cmd.
	// 3. & | " ' 기호가 cmd 위치에 존재할 경우 에러
	// 4. * ? 는 찾기 전에 미리 변환. 즉, exec 단계가 아니라 여기서 완료해야 함.
	// 5. 
	flag = 0;
	ret.head = NULL;
	ret.tail = NULL;
	quote = 0;
	is_cmd = 0;
	dup = strdup(line); // line cannot be modified.
	i = 0;
	ret.head = NULL;
	ret.tail = NULL;
	// cmd loop
	// && 등으로 분리되는 명령마다 한 번 씩.
	//printf("%s\n", line);
	while (check_set(*(dup + i), set))
		i++;
	while (*(dup + i))
	{
		is_cmd = 0;
		// word loop
		end_cmd = 1;
		// 띄어쓰기나 "" 등으로 분리되는 단어마다 한 번 씩
		while (end_cmd && *(dup + i))
		{
			flag = 0;
			while (check_set(*(dup + i), set))
			{
				i++;
			}
			if (is_cmd == 0 && check_set(*(dup + i), "&|<>"))
			{
				printf("parse error near %c\n", *(dup + i));
				// parse error near '<error message>'
				// actual : error message
				// & : &
				// && : &&
				// &&& : &&
				// .... : &&
				// | is same case.
				// < : \n
				// > : \n
				// >> : \n
				return (ret);
			}
			else if (check_set(*(dup + i), "&|<>"))
			{
				if (*(dup + i) == '&' && *(dup + i + 1) == '&')
				{
					desc = 12;
					word = strdup("&&");
					is_cmd = 0;
					end_cmd = 0;
					i += 1;
				}
				else if (*(dup + i) == '&')
				{
					desc = 14;
					word = strdup("&");
					is_cmd = 0;
					end_cmd = 0;
				}
				else if (*(dup + i) == '|' && *(dup + i + 1) == '|')
				{
					desc = 13;
					word = strdup("||");
					is_cmd = 0;
					end_cmd = 0;
					i += 1;
				}
				else if (*(dup + i) == '|')
				{
					desc = 11;
					word = strdup("|");
					is_cmd = 0;
					end_cmd = 0;
				}
				else if (*(dup + i) == '>' && *(dup + i + 1) == '>')
				{
					desc = 9;
					word = ">>";
					i += 2;
					while (check_set(*(dup + i), set))
						i++;
					pptr = dup + i;
					while (!check_set(*(dup + i), set))
						i++;
					word = strdup_ptr(pptr, dup + i);
					desc = 10;
				}
				else if (*(dup + i) == '>')
				{
					desc = 5;
					word = "<";
					i += 1;
					while (check_set(*(dup + i), set))
						i++;
					pptr = dup + i;
					while (!check_set(*(dup + i), set))
						i++;
					word = strdup_ptr(pptr, dup + i);
					desc = 6;
				}
				else if (*(dup + i) == '<')
				{
					desc = 7;
					word = "<";
					i += 1;
					while (check_set(*(dup + i), set))
						i++;
					pptr = dup + i;
					while (!check_set(*(dup + i), set))
						i++;
					word = strdup_ptr(pptr, dup + i);
					desc = 8;
				}
			}
			else if (check_set(*(dup + i), "\'\""))
			{
				quote = *(dup + i);
				i++;
				pptr = dup + i;
				while (*(dup + i) != quote)
					i++;
				word = strdup_ptr(pptr, dup + i);
				if (quote == '\'')
					desc = 3;
				else if (quote == '\"')
					desc = 2;
				quote = 0;
				if (!is_cmd)
				{
					is_cmd = 1;
					desc = 1;
				}

			}
			else
			{
				pptr = dup + i;
				while (!check_set(*(dup + i), set))
					i++;
				word = strdup_ptr(pptr, dup + i);
				DIR				*mydir;
   				struct dirent	*myfile;
				char			buf[512];
				int				i;

				mydir = opendir(".");
				i = 0;
				desc = 4;
				if (!is_cmd)
				{
					is_cmd = 1;
					desc = 1;
				}
				wildcard = 0;
				while (*(word + i))
				{
					if (check_set(*(word + i), "*?"))
						wildcard = 1;
					i++;
				}
				if (wildcard)
				{
					while ((myfile = readdir(mydir)) != NULL)
					{
						if (strcmp(myfile->d_name, word) && !fnmatch(word, myfile->d_name, 0))
						{
							desc = 4;
							if (!is_cmd)
							{
								is_cmd = 1;
								desc = 1;
							}
							if (ret.head == NULL)
							{
								t_node 	*node;

								node = malloc(sizeof(t_node));
								node->desc = desc;
								node->word = myfile->d_name;
								node->next = NULL;
								ret.head = node;
								ret.tail = node;
							}
							else
							{
								t_node	*node;

								node = malloc(sizeof(t_node));
								node->desc = desc;
								node->word = myfile->d_name;
								node->next = NULL;
								ret.tail->next = node;
								ret.tail = node;
							}
							flag = 1;
						}
					}
				}
				if (flag == 0)
				{
					dprintf(STDERR_FILENO, "minish: no matches found: %s\n", word);
					quit = 1;
				}
			}
			if (!wildcard && ret.head == NULL)
			{
				t_node 	*node;

				node = malloc(sizeof(t_node));
				node->desc = desc;
				node->word = word;
				node->next = NULL;
				ret.head = node;
				ret.tail = node;
			}
			else if (!wildcard)
			{
				t_node	*node;

				node = malloc(sizeof(t_node));
				node->desc = desc;
				node->word = word;
				node->next = NULL;
				ret.tail->next = node;
				ret.tail = node;
			}
			i++;
			while (check_set(*(dup + i), set))
				i++;
		}
	}
	
	return (ret);
}