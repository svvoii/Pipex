/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multiple_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/20 12:27:29 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_multiple_child_ex(t_pipex *current, char **av, char **arr, int ac);
int		ft_first(t_pipex *current);
int		ft_last(t_pipex *current);
void	ft_exec_cmd(t_pipex *current, char **av, char **env, int ac);
int		ft_error(char *path, char *command);

void	ft_multiple_child_ex(t_pipex *current, char **av, char **arr, int ac)
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
		ft_putstr_fd(": No such file or directory \n", 2);
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
		ft_putstr_fd(": permisson denied\n", 2);
		return (0);
	}
	dup2(fd, 1);
	dup2(current->rd_in, 0);
	close(fd);
	close(current->rd_in);
	return (1);
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
		ft_multiple_child_ex(current, av, arr, ac);
	ft_free(arr);
	return ;
}

int	ft_error(char *path, char *command)
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
