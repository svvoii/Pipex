# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 13:35:35 by sbocanci          #+#    #+#              #
#    Updated: 2023/03/10 13:53:17 by sbocanci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES	= sources/pipex.c \
		libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strlen.c libft/ft_strnstr.c

OBJS	= ${FILES:.c=.o}

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
