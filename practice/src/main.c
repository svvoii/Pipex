/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/18 17:17:49by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define GNL_BUF_SIZE 1024

typedef struct s_pipex
{
	char			*file_name;
	char			*cmd;
	int				rd_in;
	int				wr_out;
	int				pos;
	struct s_pipex	*next;
}	t_pipex;

/* main */
void	ft_here_doc(char **av);
void	ft_execve(char **av, char **env, int order, int fd[2]);
void	ft_multiple_pipes(int ac,char **av, char **env);
/* execution if here_doc && 2cmds */
void	ft_path(char *av, char **path, char **env);
char	*ft_check_bin(char *cmd, char *command);
char	**ft_set_arr(char *path, char *av);
void	ft_exec_child(char **av, char **arr, int fd[2], int order);
int		ft_second_cmd(int fd[2], char *file);
int		ft_first_cmd(int fd[2]);
void    ft_free(char **arr);
/* muliple pipes */
t_pipex	*ft_init(int pos, char *file);
void	ft_pipe(t_pipex *current, int ac, int i, char *file);
void	ft_exec_cmd(t_pipex *current, char **av, char **env, int ac);
void	ft_free_struc(t_pipex *current);
void	ft_child_exec(t_pipex *current, char **av, char **arr, int ac);
int		ft_first(t_pipex *current);
int		ft_last(t_pipex *current);
int		ft_error(char *path, char *command);
void	ft_error_norm(int fd, char *path, char *command);
/* libft helpers */
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *s, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putendl_fd(char *s, int fd);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
/* ..split */
char	**ft_split(char const *s, char c);
void	to_split(char **spt, char const *s, char c);
void	copy_word(char **spt, char const *s, size_t word, size_t len);
/* ..get_next_line */
int		get_next_line(int fd, char **line);
void	paste_line(char **readed_line, char *buff);
int		result(char **line, char **readed_line, int fd, int readed);
void	ft_strdel(char **str);

int	main(int ac, char **av, char **env)
{
	int	fd[2];
	
	if (ac < 5)
	{
		write(2, "Error: Invalid arguments\n", 25);
		write(2, "Use: ./pipex <file1> <cmd1> <cmd2> .. <file2>\n", 46);
		return (1);
	}
	if (!ft_strncmp(av[1], "here_doc", ft_strlen(av[1])))
	{
		if (ac == 6)
		{
			ft_here_doc(av);
			pipe(fd);
			ft_execve(av, env, 0, fd);
			close(fd[1]);
			ft_execve(av, env, 1, fd);
			close(fd[0]);
			while (wait(0) > 0)
			{}
		}
	}
	else
		ft_multiple_pipes(ac, av, env);
	return (0);
}

void	ft_here_doc(char **av);
void	ft_execve(char **av, char **env, int order, int fd[2]);
void	ft_multiple_pipes(int ac,char **av, char **env);

void	ft_here_doc(char **av)
{
	int		fd;
	char	*buffer;

	buffer = NULL;
	fd = open("/tmp/herDoc", O_RDONLY | O_WRONLY | O_CREAT, 0644);
	while (get_next_line(0, &buffer) > 0)
	{
		if (!ft_strncmp(buffer, av[2], ft_strlen(buffer)))
			break ;
		else
			ft_putendl_fd(buffer, fd);
		free(buffer);
	}
	free(buffer);
}

void	ft_execve(char **av, char **env, int order, int fd[2])
{
	char	*path;
	char	**arr;
	pid_t	pid;

	arr = NULL;
	path = NULL;
	if (order == 0)
	{
		ft_path(av[3], &path, env);
		arr = ft_set_arr(path, av[3]);
	}
	else if (order == 1)
	{
		ft_path(av[4], &path, env);
		arr = ft_set_arr(path, av[4]);
	}
	pid = fork();
	if (pid == 0)
		ft_exec_child(av, arr, fd, order);
	ft_free(arr);
	return ;
}

void	ft_multiple_pipes(int ac,char **av, char **env)
{
	t_pipex	*current;
	t_pipex	*previous;
	int		i;

	i = 1;
	current = ft_init(2, av[i]);
	while (++i < ac - 1)
	{
		current->cmd = av[i];
		if (i != ac - 2)
			ft_pipe(current, ac - 1, i, av[ac - 1]);
		ft_exec_cmd(current, av, env, ac);
		if (current->wr_out != 1)
			close(current->wr_out);
		if (current->rd_in != 0)
			close(current->rd_in);
		previous = current;
		current = current->next;
		ft_free_struc(previous);
	}
	while (wait(0) > 0)
	{}
}

/* execution if here_doc && 2cmds */
void	ft_path(char *av, char **path, char **env)
{
	int		i;
	char	**cmd;
	char	*tmp;
	char	**command;

	i = 0;
	tmp = NULL;
	command = ft_split(av, ' ');
	while (env && env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	cmd = ft_split(env[i], ':');
	if (command[0] && command[0][0] == '/' && !(*path))
		*path = ft_strdup(command[0]);
	else if (cmd)
	{
		*path = ft_check_bin(cmd[0] + 5, command[0]);
		i = 0;
		while (command[0] && cmd[i] && *path == NULL)
			*path = ft_check_bin(cmd[i++], command[0]);
	}
	else if (*path == NULL && !cmd)
		*path = ft_strdup("error");
	free(tmp);
	ft_free(command);
	ft_free(cmd);
}

char	*ft_check_bin(char *cmd, char *command)
{
	int		fd;
	char	*path;
	char	*com;
	char	*tmp;

	path = NULL;
	tmp = ft_strjoin(cmd, "/");
	com = ft_strjoin(tmp, command);
	fd = open(com, O_RDONLY);
	if (fd == -1)
	{
		free(tmp);
		free(com);
		return (NULL);
	}
	path = com;
	close(fd);
	free(tmp);
	return (path);
}

char	**ft_set_arr(char *path, char *av)
{
	int		j;
	char	**arr;
	char	*arg;
	char	*cmd;

	j = 1;
	arg = NULL;
	cmd = av;
	if (cmd[0] != '\0')
	{
		arr = ft_split(cmd, ' ');
		free(arr[0]);
		arr[0] = path;
	}
	else
	{
		arr = malloc(sizeof (char **) * 1024);
		arr[0] = ft_strdup("err");
	}
	return (arr);
}

/* This exec_child used in ft_execve() used in main (when there is here_doc and ac == 6)*/
void	ft_exec_child(char **av, char **arr, int fd[2], int order)
{
	int	ret;

	ret = 0;
	if (!order)
		ret = ft_first_cmd(fd);
	else
		ret = ft_second_cmd(fd, av[5]);
	if (ret == 1)
	{
		execve(arr[0], arr, NULL);
		if (!order)
			ret = ft_error(arr[0], av[3]);
		else
			ret = ft_error(arr[0], av[4]);
		exit(ret);
	}
	exit(1);
}

int    ft_first_cmd(int fd[2])
{
    int    fd_file;

    fd_file = open("/tmp/herDoc", O_RDONLY);
    dup2(fd_file, 0);
    dup2(fd[1], 1);
    close(fd_file);
    unlink("/tmp/herDoc");
    close(fd[0]);
    return (1);
}

int    ft_second_cmd(int fd[2], char *file)
{
    int    fd_file;

    fd_file = open(file, O_CREAT | O_APPEND| O_RDWR, 0644);
    if (fd_file == -1)
    {
        ft_putstr_fd("wsh: ", 0);
        ft_putstr_fd(file, 0);
        ft_putendl_fd(": permisson denied", 0);
        return (0);
    }
    dup2(fd_file, 1);
    dup2(fd[0], 0);
    close(fd_file);
    close(fd[0]);
    close(fd[1]);
    return (1);
}

void    ft_free(char **arr)
{
    int    i;

    i = 0;
    if (arr != NULL)
    {
        while (arr[i] && arr[i][0] != '\0')
        {
            free(arr[i++]);
            arr[i - 1] = NULL;
        }
        free(arr);
        arr = NULL;
    }
    return ;
}

/* muliple pipes */
t_pipex	*ft_init(int pos, char *file)
{
	t_pipex	*tmp;

	tmp = (t_pipex *)malloc(sizeof(t_pipex));
	tmp->rd_in = 0;
	tmp->file_name = file;
	tmp->pos = pos;
	tmp->wr_out = 1;
	return (tmp);
}

void	ft_pipe(t_pipex *current, int ac, int i, char *file)
{
	int	fd[2];

	if (i + 2 == ac)
		current->next = ft_init(i + 1, file);
	else
		current->next = ft_init(i + 1, "");
	pipe(fd);
	current->wr_out = fd[1];
	current->next->rd_in = fd[0];
}

void	ft_exec_cmd(t_pipex *current, char **av, char **env, int ac)
{
	pid_t	pid;
	char	*path;
	char	**arr;

	arr = NULL;
	path = NULL;
	ft_path(current->cmd, &path, env);
	arr = ft_set_arr(path, current->cmd);
	pid = fork();
	if (pid == 0)
		ft_child_exec(current, av, arr, ac);
	ft_free(arr);
	return ;
}

void	ft_free_struc(t_pipex *struc)
{
	if (struc)
	{
		free(struc);
		struc = NULL;
	}
}

/* ..from ft_exec_cmd() used in ft_multiple_pipes() */
void	ft_child_exec(t_pipex *current, char **av, char **arr, int ac)
{
	int	ret;

	ret = 0;
	if (current->pos == 2)
		ret = ft_first(current);
	if (current->rd_in != 0 && current->pos != ac - 2 && current->pos != 2)
		dup2(current->rd_in, 0);
	if (current->wr_out != 1 && current->pos != ac - 2 && current->pos != 2)
	{
		close(current->next->rd_in);
		dup2(current->wr_out, 1);
		ret = 1;
	}
	else if (current->pos == ac - 2)
		ret = ft_last(current);
	if (ret == 1)
	{
		execve(arr[0], arr, NULL);
		ft_error(arr[0], av[current->pos]);
		exit(ret);
	}
	exit(1);
}

int	ft_first(t_pipex *current)
{
	int	fd;

	fd = open(current->file_name, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("err: ", 2);
		ft_putstr_fd(current->file_name, 2);
		ft_putendl_fd(": No such file or directory ", 2);
		return (0);
	}
	dup2(fd, 0);
	dup2(current->wr_out, 1);
	close(fd);
	close(current->wr_out);
	close(current->next->rd_in);
	return (1);
}

int	ft_last(t_pipex *current)
{
	int	fd;

	fd = open(current->file_name, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("err: ", 2);
		ft_putstr_fd(current->file_name, 2);
		ft_putendl_fd(": permisson denied", 2);
		return (0);
	}
	dup2(fd, 1);
	dup2(current->rd_in, 0);
	close(fd);
	close(current->rd_in);
	return (1);
}

int    ft_error(char *path, char *command)
{
	int	fd;
    int	ret;

    fd = open(path, O_WRONLY);
    ft_error_norm(fd, path, command);
    if ((path && ft_strchr(path, '/') == NULL) || (fd == -1))
        ret = 127;
    else
        ret = 126;
    close(fd);
    return (ret);
}

void	ft_error_norm(int fd, char *path, char *command)
{
	int	i;

	i = 0;
	ft_putstr_fd("err: ", 2);
	if (path && !(ft_strncmp(path, "error", 6)))
	{
		ft_putstr_fd(command, 2);
		ft_putendl_fd(": No such file or directory", 2);
		i = 1;
	}
	else if (fd == -1)
		path = command;
	if (i == 0)
		ft_putstr_fd(path, 2);
	if ((ft_strchr(path, '/')) == NULL && i == 0)
		ft_putendl_fd(": command not found", 2);
	else if (fd == -1 && i == 0)
		ft_putendl_fd(": No such file or directory", 2);
	else if (fd == -1 && i == 0)
		ft_putendl_fd(": is a directory", 2);
	else if (fd != -1 && i == 0)
		ft_putendl_fd(": Permission denied", 2);
}

/* ** libft ** */
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s || !fd)
		return ;
	write(fd, s, ft_strlen(s));
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return (0);
	else if ((*s1 != *s2) || !*s1)
		return ((unsigned char)*s1 - (unsigned char)*s2);
	return (ft_strncmp(++s1, ++s2, --n));
}

void	ft_putendl_fd(char *s, int fd)
{
	if (!s || !fd)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

char	*ft_strchr(const char *s, int c)
{
	unsigned char	uc;

	uc = c;
	while (*s != '\0')
	{
		if (*s == uc)
			return ((char *)s);
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
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

char	*ft_strdup(const char *s)
{
	size_t		i;
	size_t		s_len;
	char		*dup;

	s_len = ft_strlen((char *)s);
	dup = malloc(s_len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < s_len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

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

int	result(char **line, char **readed_line, int fd, int readed)
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

void	ft_strdel(char **str)
{
	if (str)
	{
		free(*str);
		*str = NULL;
	}
}
