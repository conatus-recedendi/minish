/*
**	scan.c
**
*/

#include "scan.h"

char		*line_append(const char *des, const char *append)
{
	int		dlen;
	int		alen;
	int		i;
	char	*ret;

	dlen = strlen(des);
	alen = strlen(append);
	ret = malloc(sizeof(char) * (alen + dlen + 1));
	if (ret == NULL)
		return (NULL);
	ret[alen + dlen] = 0;
	i = 0;
	while (i < dlen)
	{
		ret[i] = des[i]; 
		i++;
	}
	while (i < alen + dlen)
	{
		ret[i] = append[i - dlen];
		i++;
	}
	return (ret);
}

char		*get_line(int fd)
{
	char	*ret;
	int		temp;

	ret = malloc(sizeof(char) * (SCAN_LEN_MAX + 1));
	ret[SCAN_LEN_MAX] = 0;
	if (read(fd, ret, SCAN_LEN_MAX) == -1)
	{
		ret = NULL;
	}
	return (ret);
}