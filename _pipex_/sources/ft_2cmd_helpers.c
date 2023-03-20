/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_2cmd_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/20 12:30:43 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_here_doc(char **av);
void	ft_few_child_ex(char **av, char **arr, int fd[2], int order);
int		ft_second_cmd(int fd[2], char *file);
int		ft_first_cmd(int fd[2]);
void	ft_error_norm(int fd, char *path, char *command);

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
		{
			ft_putstr_fd(buffer, fd);
			write(fd, "\n", 1);
		}
		free(buffer);
	}
	free(buffer);
}

void	ft_few_child_ex(char **av, char **arr, int fd[2], int order)
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

int	ft_first_cmd(int fd[2])
{
	int	fd_file;

	fd_file = open("/tmp/herDoc", O_RDONLY);
	dup2(fd_file, 0);
	dup2(fd[1], 1);
	close(fd_file);
	unlink("/tmp/herDoc");
	close(fd[0]);
	return (1);
}

int	ft_second_cmd(int fd[2], char *file)
{
	int	fd_file;

	fd_file = open(file, O_CREAT | O_APPEND | O_RDWR, 0644);
	if (fd_file == -1)
	{
		ft_putstr_fd("wsh: ", 0);
		ft_putstr_fd(file, 0);
		ft_putstr_fd(": permisson denied\n", 0);
		return (0);
	}
	dup2(fd_file, 1);
	dup2(fd[0], 0);
	close(fd_file);
	close(fd[0]);
	close(fd[1]);
	return (1);
}

void	ft_error_norm(int fd, char *path, char *command)
{
	int	i;

	i = 0;
	ft_putstr_fd("err: ", 2);
	if (path && !(ft_strncmp(path, "error", 6)))
	{
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		i = 1;
	}
	else if (fd == -1)
		path = command;
	if (i == 0)
		ft_putstr_fd(path, 2);
	if ((ft_strchr(path, '/')) == NULL && i == 0)
		ft_putstr_fd(": command not found\n", 2);
	else if (fd == -1 && i == 0)
		ft_putstr_fd(": No such file or directory\n", 2);
	else if (fd == -1 && i == 0)
		ft_putstr_fd(": is a directory\n", 2);
	else if (fd != -1 && i == 0)
		ft_putstr_fd(": Permission denied\n", 2);
}
