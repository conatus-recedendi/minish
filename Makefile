
NAME		=	minish
CC			=	gcc
CFLAGS		=	
PATH_HEAD	=	./includes/
PATH_SRCS	=	./srcs/
PATH_BIN	=	./srcs/bin/

ORIG_HEAD	=	minish.h \
				print.h \
				prompt.h \
				scan.h \
				lex.h \
				split.h \
				exec.h \
				pipe.h

HEAD		=	${addprefix ${PATH_HEAD}, ${ORIG_HEAD}}
ORIG_SRCS	=	minish.c \
				tools/print.c \
				tools/scan.c \
				prompt.c \
				lex.c \
				tools/split.c \
				exec.c \
				pipe.c

BIN			=	cat.c \
				wc.c \
				ls.c

SRCS		=	${addprefix ${PATH_SRCS}, ${ORIG_SRCS}}
OBJS		=	${SRCS:.c=.o}
				
.PHONY		:	all clean fclean re

all			:	${NAME} bin

${NAME}		:	${OBJS}
				${CC} -o $@ ${OBJS} -I${PATH_HEAD}

${BIN}		:
				${CC} -I ${PATH_HEAD} ${addprefix ${PATH_BIN}, $@} -o ${addprefix ${PATH_BIN}, $*}

bin			:	${BIN}


%.o			:	%.c
				${CC} ${CFLAGS} -c $< -o $@ -I${PATH_HEAD}

clean		:
				rm -f ${OBJS}

fclean		:	clean
				rm -f ${NAME}

re			:	fclean ${NAME}
