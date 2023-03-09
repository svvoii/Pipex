
FILES		= ft_pipex ft_pipex_heredoc ft_execution \
				ft_libft ft_split ft_get_next_line

FILES		:= $(addprefix src/, ${FILES})
FILES		:= $(addsuffix .c, ${FILES})
OBJS		= ${FILES:.c=.o}

NAME		= pipex

CC			= gcc -Wall -Wextra -Werror

INCLUDES	= -I./includes

SYSTEM		= $(shell uname -s)

${NAME}:	${OBJS}
			${CC} ${INCLUDES} -o ${NAME} ${OBJS}

.c.o:
			${CC} -c ${INCLUDES} $< -o ${<:.c=.o}

all:		${NAME}

profile:	fclean
profile:	CC := ${CC} -pg
profile:	${NAME}

clean:
			rm -f ${OBJS}

fclean:		clean
			rm -f ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
