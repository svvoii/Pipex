/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_2cmd_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/20 12:28:53 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_execve(char **av, char **env, int order, int fd[2]);
void	ft_path(char *av, char **path, char **env);
char	*ft_check_bin(char *cmd, char *command);
char	**ft_set_arr(char *path, char *av);
void	ft_free(char **arr);

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
		ft_few_child_ex(av, arr, fd, order);
	ft_free(arr);
	return ;
}

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
	char	**arr;
	char	*cmd;

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

void	ft_free(char **arr)
{
	int	i;

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
