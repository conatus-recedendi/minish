/*
**	ls.c
**
**	This code have been referenced by someone else's code.
**	But Some parts are edited to match the rules for my shell.
**	https://stackoverflow.com/questions/20450556/linux-ls-l-implementation-using-file-descriptors-in-c-c
**	by Arunbalaji
*/

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

int		exitn;
void	print_ls(char *path)
{
    DIR				*mydir;
    struct dirent	*myfile;
    struct stat		mystat;
	struct passwd	*pw;
	struct group	*gp;
	char			*c;
	char			buf[512];
	int				i;
	int				status;
	struct stat		st_buf;

	status = stat(path, &st_buf);
	mydir = opendir(path);
	if (!S_ISDIR(st_buf.st_mode) && !S_ISREG(st_buf.st_mode) )
	{
		dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", "ls", path);
		exitn = 1;
		return ;
	}
	else if (S_ISDIR(st_buf.st_mode))
	{
		while((myfile = readdir(mydir)) != NULL)
		{
			sprintf(buf, "%s/%s", path, myfile->d_name);
			stat(buf, &mystat);
			printf( (S_ISDIR(mystat.st_mode)) ? "d" : "-");
			printf( (mystat.st_mode & S_IRUSR) ? "r" : "-");
			printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
			printf( (mystat.st_mode & S_IXUSR) ? "x" : "-");
			printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");
			printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
			printf( (mystat.st_mode & S_IXGRP) ? "x" : "-");
			printf( (mystat.st_mode & S_IROTH) ? "r" : "-");
			printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
			printf( (mystat.st_mode & S_IXOTH) ? "x" : "-");
			printf(" ");
			printf("%3d ",mystat.st_nlink);
			pw = getpwuid(mystat.st_uid);
			printf("%s  ", pw->pw_name);
			gp = getgrgid(mystat.st_gid);
			printf("%s ", gp->gr_name);
			printf("%6lld ",mystat.st_size);
			c = ctime(&mystat.st_mtimespec.tv_sec);
			i = 4;
			while (i < 16)
				printf("%c", c[i++]);
			printf(" ");
			printf("%s\n",myfile->d_name);
			fflush(stdout);
		}
		closedir(mydir);
	}
	else if (S_ISREG(st_buf.st_mode))
	{
		stat(path, &mystat);
		printf( (S_ISDIR(mystat.st_mode)) ? "d" : "-");
		printf( (mystat.st_mode & S_IRUSR) ? "r" : "-");
		printf( (mystat.st_mode & S_IWUSR) ? "w" : "-");
		printf( (mystat.st_mode & S_IXUSR) ? "x" : "-");
		printf( (mystat.st_mode & S_IRGRP) ? "r" : "-");
		printf( (mystat.st_mode & S_IWGRP) ? "w" : "-");
		printf( (mystat.st_mode & S_IXGRP) ? "x" : "-");
		printf( (mystat.st_mode & S_IROTH) ? "r" : "-");
		printf( (mystat.st_mode & S_IWOTH) ? "w" : "-");
		printf( (mystat.st_mode & S_IXOTH) ? "x" : "-");
		printf(" ");
		printf("%3d ",mystat.st_nlink);
		pw = getpwuid(mystat.st_uid);
		printf("%s  ", pw->pw_name);
		gp = getgrgid(mystat.st_gid);
		printf("%s ", gp->gr_name);
		printf("%6lld ",mystat.st_size);
		c = ctime(&mystat.st_mtimespec.tv_sec);
		i = 4;
		while (i < 16)
		{
			printf("%c", c[i]);
			i++;
		}
		printf(" ");
		printf("%s \n", path);
		//fclose(file);
	}
}

void		print_n(char *path)
{
    DIR				*mydir;
    struct dirent	*myfile;
	char			buf[512];
	int				i;
	FILE			*file;
	int				status;
	struct stat		st_buf;

	status = stat(path, &st_buf);
	mydir = opendir(path);
	dprintf(STDERR_FILENO, "%s %d: %d:\n", path, S_ISDIR(st_buf.st_mode), S_ISREG(st_buf.st_mode));
	if (!S_ISDIR(st_buf.st_mode) && !S_ISREG(st_buf.st_mode) )
	{
		dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", "ls", path);
		exitn = 1;
		return ;
	}
	else if (S_ISDIR(st_buf.st_mode))
	{
		i = 0;
		while((myfile = readdir(mydir)) != NULL)
		{
			sprintf(buf, "%s/%s", path, myfile->d_name);
			printf("%-15s",myfile->d_name);
			i++;
			if ((i % 4) == 0)
				printf("\n");
		}
		printf("\n");
		closedir(mydir);
	}
	else if (S_ISREG(st_buf.st_mode))
	{
		printf("%s\n", path);
	}
}

int main(int argc, char *argv[])
{
	int				i;
	int				flag;
	struct stat		st_buf;
	int				status;

	i = 1;
	if (argc == 1)
	{
		print_n(".");
		printf("\n");
		exit(exitn);
	}
	else if (!strcmp(argv[1], "-l"))
	{
		flag = 1;
		if (argc == 2)
		{
			print_ls(".");
			printf("\n");
			exit(exitn);
		}
		i++;
	}
	if (flag)
	{
		if (argc >= 4)
		{
			i = 2;
			while (i < argc)
			{
				status = stat(argv[i], &st_buf);
				if (S_ISDIR(st_buf.st_mode))
				{
					printf("%s:\n", argv[i]);
					printf("total %d\n", i);
				}
				print_ls(argv[i]);
				if (S_ISDIR(st_buf.st_mode))
					printf("\n\n");
				i++;
			}
			// path:\n total ~~~
		}
		else
		{
			print_ls(argv[2]);
		}
	}
	else
	{
		i = 1;
		while (i < argc)
		{
			status = stat(argv[i], &st_buf);
			if (argc >= 3 && S_ISDIR(st_buf.st_mode))
				printf("%s:\n", argv[i]);
			print_n(argv[i]);
			if (argc >= 3 && S_ISDIR(st_buf.st_mode))
				printf("\n\n");
			i++;
		}
	}
	exit(exitn);
}