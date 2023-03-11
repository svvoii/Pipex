# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sv <sv@student.42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/10 13:35:35 by sbocanci          #+#    #+#              #
#    Updated: 2023/03/11 10:44:39 by sv               ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES	= sources/ft_pipex.c sources/ft_child.c sources/ft_files.c sources/ft_free.c \
		libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strlen.c \
		libft/ft_strnstr.c libft/ft_strdup.c libft/ft_strchr.c libft/ft_substr.c\
		libft/get_next_line.c
#FILES	= sources/pipex.c \
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
