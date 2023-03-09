#include <../includes/pipex.h>

static char		*append_line(char **line, char c);
static ssize_t	read_file(int fd, size_t *i, char *buff);

#define BUFFER_SIZE 1024

char	*get_next_line(int fd)
{
	static char		buff[BUFFER_SIZE + 1] = "";
	static size_t	i = 0;
	int				buffend;
	char			*line;

	if ((!buff[i] && read_file(fd, &i, buff) <= 0) || !append_line(&line, 0))
		return (0);
	buffend = -1;
	while (buffend)
	{
		buffend = 0;
		while (buff[i] && i <= BUFFER_SIZE)
		{
			if (!append_line(&line, buff[i++]))
				return (0);
			if (buff[i - 1] == '\n' && buff[i--] + 1)
				break ;
		}
		if (!buff[i])
			if (read_file(fd, &i, buff) > 0)
				buffend = 1;
	}
	i++;
	return (line);
}

static ssize_t	read_file(int fd, size_t *i, char *buff)
{
	ft_bzero(buff, (size_t) sizeof(char) * (BUFFER_SIZE));
	*i = 0;
	return (read(fd, buff, (size_t)BUFFER_SIZE));
}

static char	*append_line(char **line, char c)
{
	char	*newln;
	char	s[2];

	s[0] = c;
	s[1] = 0;
	if (!c)
	{
		*line = ft_calloc(1, sizeof(char));
		if (!*line)
			return (0);
		return (*line);
	}
	newln = ft_strjoin(*line, s);
	free(*line);
	*line = newln;
	return (newln);
}
