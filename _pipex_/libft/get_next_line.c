/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:48:21 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/13 17:22:41 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int			get_next_line(int fd, char **line);
void		paste_line(char **readed_line, char *buff);
static int	result(char **line, char **readed_line, int fd, int readed);
static void	ft_strdel(char **str);

int	get_next_line(int fd, char **line)
{
	static char	*readed_line[64];
	char		*buff;
	int			readed;

	if (fd < 0 || line == NULL || GNL_BUF_SIZE < 1)
		return (-1);
	buff = (char *)malloc((GNL_BUF_SIZE + 1) * sizeof(char));
	if (!(buff))
		return (-1);
	readed = 1;
	while (readed > 0)
	{
		readed = read(fd, buff, GNL_BUF_SIZE);
		buff[readed] = '\0';
		if (!(readed_line[fd]))
			readed_line[fd] = ft_strdup(buff);
		else
			paste_line(&readed_line[fd], buff);
		if (ft_strchr(buff, '\n'))
			break ;
	}
	free(buff);
	return (result(line, readed_line, fd, readed));
}

void	paste_line(char **readed_line, char *buff)
{
	char		*tmp;

	tmp = ft_strjoin(*readed_line, buff);
	free(*readed_line);
	*readed_line = tmp;
}

static int	result(char **line, char **readed_line, int fd, int readed)
{
	int		len;
	char	*tmp;

	if (readed < 0)
		return (-1);
	else if (readed == 0 && readed_line[fd] == NULL && *line)
		return (0);
	len = 0;
	while (readed_line[fd][len] != '\n' && readed_line[fd][len] != '\0')
		len++;
	if (readed_line[fd][len] == '\n')
	{
		*line = ft_substr(readed_line[fd], 0, len);
		tmp = ft_strdup(&readed_line[fd][len + 1]);
		free(readed_line[fd]);
		readed_line[fd] = tmp;
		if (readed_line[fd][0] == '\0')
			ft_strdel(&readed_line[fd]);
		return (1);
	}
	*line = ft_strdup(readed_line[fd]);
	ft_strdel(&readed_line[fd]);
	return (0);
}

static void	ft_strdel(char **str)
{
	if (str)
	{
		free(*str);
		*str = NULL;
	}
}
