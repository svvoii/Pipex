/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/17 15:22:01 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

void	listen(char *limiter);
void	in_process(char *infile);
void	do_pipe(char *cmd, char **envp);
void	out_process(char *outfile, char *cmd, char **envp);
char	*find_path(char **envp, char *cmd);
void	puterror(char *str);
/* helpers */
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
/* ..split */
char	**ft_split(char const *s, char c);
void	to_split(char **spt, char const *s, char c);
void	copy_word(char **spt, char const *s, size_t word, size_t len);
/* ..get_next_line */
char	*get_next_line(int fd);
ssize_t	read_file(int fd, size_t *i, char *buff);
char	*append_line(char **line, char c);

int	main(int ac, char **av, char **envp)
{
	int		index;

	if (ac < 5)
	{
		char	use_01[] = "Usage: ./pipex filein cmd1 cmd2 cmdN ... fileout\n";
		char	use_02[] = "Usage: ./pipex here_doc LIMITER cmdN ... fileout\n";
		write(2, use_01, strlen(use_01));
		write(2, use_02, strlen(use_02));
		return (0);
	}
	index = 2;
	if (!strncmp(av[1], "here_doc", 8))
	{
		listen(av[2]);
		index = 3;
	}
	else
		in_process(av[1]);
	while (index < ac - 2)
		do_pipe(av[index++], envp);
	out_process(av[ac - 1], av[ac - 2], envp);
	return (0);
}

void	listen(char *limiter)
{
	char	*line;
	int		fds[2];
	int		pid;

	if (pipe(fds) == -1)
		puterror("Pipe Error ");
	pid = fork();
	if (pid == 0)
	{
		while (1)
		{
			line = get_next_line(0);
			if (ft_strnstr(line, limiter, strlen(limiter)))
				exit(-1);
			write(fds[1], line, strlen(line));
		}
	}
	else
	{
		dup2(fds[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
	close(fds[0]);
	close(fds[1]);
}

void	in_process(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
		puterror("Infile Error ");
	dup2(fd, STDIN_FILENO);
}

void	do_pipe(char *cmd, char **envp)
{
	int		fds[2];
	int		pid;
	char	*path;

	if (pipe(fds) == -1)
		puterror("Pipe Error ");
	pid = fork();
	if (pid == -1)
		puterror("Fork Error ");
	if (pid == 0)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		path = find_path(envp, cmd);
		if (!path || execve(path, ft_split(cmd, ' '), envp) == -1)
			puterror("Execve Error command failed/not founded ");
	}
	else
	{
		close(fds[1]);
		wait(NULL);
		dup2(fds[0], STDIN_FILENO);
	}
}

void	out_process(char *outfile, char *cmd, char **envp)
{
	int		fd;
	int		execstat;
	char	*path;

	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		puterror("Infile Error ");
	dup2(fd, STDOUT_FILENO);
	path = find_path(envp, cmd);
	if (!path)
		puterror("Command not found ");
	execstat = execve(path, ft_split(cmd, ' '), envp);
	if (execstat == -1)
		puterror("Execve Error command failed/not founded ");
}

char	*find_path(char **envp, char *cmd)
{
	char	**paths;
	char	*cmd_path;
	char	*sub;

	cmd = *ft_split(cmd, ' ');
	while (*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	sub = ft_substr(*envp, 5, strlen(*envp) - 5);
	paths = ft_split(sub, ':');
	free(sub);
	sub = ft_strjoin("/", cmd);
	while (*paths)
	{
		cmd_path = ft_strjoin(*paths, sub);
		if (!cmd_path)
			return (NULL);
		if (!access(cmd_path, F_OK))
			break ;
		free(cmd_path);
		paths++;
	}
	free(sub);
	free(cmd);
	return (cmd_path);
}

void	puterror(char *str)
{
	write(2, "\033[0;31m", 8);
	perror(str);
	write(2, "\033[0m", 5);
	exit(-1);
}

/* helpers */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	slen;
	char	*substr;

	if (!s)
		return (NULL);
	i = 0;
	slen = strlen(s);
	substr = malloc((len + 1) * sizeof(*substr));
	if (!substr)
		return (0);
	while ((i < len) && (start + i) < slen)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t		i;
	size_t		l_len;

	l_len = strlen(little);
	i = 0;
	if (len == 0 && (!big || !little))
		return (NULL);
	if (!*little)
		return ((char *)big);
	while (big[i] && (l_len + i) <= len)
	{
		if (strncmp(big + i, little, l_len) == 0)
			return ((char *)big + i);
		i++;
	}
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s3;
	char	*tmp;

	if (!s1 || !s2)
		return (NULL);
	s3 = malloc(strlen(s1) + strlen(s2) + 1);
	if (!s3)
		return (NULL);
	tmp = s3;
	while (*s1 != '\0')
		*tmp++ = *s1++;
	while (*s2 != '\0')
		*tmp++ = *s2++;
	*tmp = '\0';
	return (s3);
}

/* split */
char	**ft_split(char const *s, char c)
{
	char		**spl;
	size_t		words;
	size_t		i;

	if (!s)
		return (NULL);
	words = 0;
	i = 0;
	while (i < strlen(s))
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			words++;
		i++;
	}
	spl = (char **)malloc(sizeof(char *) * (words + 1));
	if (!spl)
		return (NULL);
	if (words == 0)
	{
		spl[words] = (void *)0;
		return (spl);
	}
	to_split(spl, s, c);
	return (spl);
}

void	to_split(char **spt, char const *s, char c)
{
	size_t				i;
	size_t				tmp;
	size_t				word;

	word = 0;
	i = 0;
	tmp = 0;
	while (i < strlen(s))
	{
		while (s[i] == c && i < strlen(s))
			i++;
		tmp = i;
		while (s[i] != c && i < strlen(s))
			i++;
		if (i > tmp)
		{
			copy_word(spt, &s[tmp], word, i - tmp);
			word++;
		}
	}
	spt[word] = (void *)0;
}

void	copy_word(char **spt, char const *s, size_t word, size_t len)
{
	size_t			i;

	spt[word] = (char *)malloc(sizeof(*spt[word]) * (len + 1));
	if (!spt[word])
	{
		while (word--)
			free(spt[word]);
		free(spt[word]);
		free(spt);
		spt = NULL;
		return ;
	}
	i = 0;
	while (i < len && s[i])
	{
		spt[word][i] = s[i];
		i++;
	}
	spt[word][i] = '\0';
}

/* get_next_line */
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

ssize_t	read_file(int fd, size_t *i, char *buff)
{
	bzero(buff, (size_t) sizeof(char) * (BUFFER_SIZE));
	*i = 0;
	return (read(fd, buff, (size_t)BUFFER_SIZE));
}

char	*append_line(char **line, char c)
{
	char	*newln;
	char	s[2];

	s[0] = c;
	s[1] = 0;
	if (!c)
	{
		*line = calloc(1, sizeof(char));
		if (!*line)
			return (0);
		return (*line);
	}
	newln = ft_strjoin(*line, s);
	free(*line);
	*line = newln;
	return (newln);
}
