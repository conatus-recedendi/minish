/*
**	split.c
**
*/

#include "split.h"

extern int			wildcard_error;

char			*strdup_ptr(char *start, char *end)
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

int			check_set(char c, const char *set)
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
	int			is_cmd;
	int			quote;
	int			desc;
	int			end_cmd;
	int			flag;
	int			wildcard;

	flag = 0;
	ret.head = NULL;
	ret.tail = NULL;
	quote = 0;
	is_cmd = 0;
	dup = strdup(line);
	i = 0;
	ret.head = NULL;
	ret.tail = NULL;
	while (check_set(*(dup + i), set))
		i++;
	while (*(dup + i))
	{
		is_cmd = 0;
		end_cmd = 1;
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
				if (wildcard && flag == 0)
				{
					dprintf(STDERR_FILENO, "minish: no matches found: %s\n", word);
					wildcard_error = 1;
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