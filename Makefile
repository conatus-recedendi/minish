
NAME		=	minish
CC			=	gcc
CFLAGS		=	
PATH_HEAD	=	./includes/
PATH_SRCS	=	./srcs/

ORIG_HEAD	=	minish.h \
				print.h \
				prompt.h \
				scan.h \
				lex.h

HEAD		=	${addprefix ${PATH_HEAD}, ${ORIG_HEAD}}
ORIG_SRCS	=	minish.c \
				tools/print.c \
				tools/scan.c \
				prompt.c \
				lex.c
SRCS		=	${addprefix ${PATH_SRCS}, ${ORIG_SRCS}}
OBJS		=	${SRCS:.c=.o}
				
.PHONY		:	all clean fclean re

all			:	${NAME}

${NAME}		:	${OBJS}
				${CC} -o $@ ${OBJS} -I${PATH_HEAD}

%.o			:	%.c
				${CC} ${CFLAGS} -c $< -o $@ -I${PATH_HEAD}

clean		:
				rm -f ${OBJS}

fclean		:	clean
				rm -f ${NAME}

re			:	fclean ${NAME}
