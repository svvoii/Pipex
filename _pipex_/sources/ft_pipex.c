/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/17 15:57:07 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_malloc_pipes(t_pipex *pipex, int ac);
void	ft_get_envp(t_pipex *pipex, char **envp);
//void	ft_create_pipes(int pipes[1024][2]);
void	ft_close_pipes(t_pipex *pipex);

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;
	int		pipes[32][2];
	int		i;
	int		p;

	if (ac < ft_args_in(av[1], &pipex))
	{
		write(2, "Error: Invalid arguments\n", 25);
		write(2, "Use: ./pipex <file1> <cmd1> <cmd2> .. <file2>\n", 46);
		return (1);
	}
	ft_get_files(&pipex, av, av[ac - 1]);
	//ft_malloc_pipes(&pipex, ac);
	pipex.cmd_nmbs = ac - 3 - pipex.here_doc;
	i = -1;
	while (++i < pipex.cmd_nmbs - 1)
	{
		p = pipe(pipes[i]);
		if (p == -1)
			ft_free_parent(&pipex);
		printf("pipes[%d][0]:'%d'\n", i, pipes[i][0]);
		printf("pipes[%d][1]:'%d'\n", i, pipes[i][1]);
	}
	ft_get_envp(&pipex, envp);
	//ft_create_pipes(pipes);
	i = -1;
	while (++i < pipex.cmd_nmbs)
	{
		//printf("'%d'\n", i);
		ft_child(pipex, av, envp, pipes, i);
	}
	//ft_close_pipes(&pipex);
	waitpid(-1, NULL, 0);
	ft_free_parent(&pipex);
	return (0);
}

/* Calculating cmds and allocating mem for ptrs to each cmd 
void	ft_malloc_pipes(t_pipex *pipex, int ac)
{
	pipex->cmd_nmbs = ac - 3 - pipex->here_doc;
	//pipex->pipe_nmbs = 2 * (pipex->cmd_nmbs - 1);
	pipex->pipe = (int **)malloc(sizeof(int *) * (pipex->cmd_nmbs));
	if (!pipex->pipe)
		ft_error("Pipe");
}
*/
/* Will return $PATH varable from env into env_path */
/* Then split into 2d array of available env paths */
void	ft_get_envp(t_pipex *pipex, char **envp)
{
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	pipex->env_path = *envp + 5;
	pipex->cmd_paths = ft_split(pipex->env_path, ':');
	if (!pipex->cmd_paths)
		ft_free_pipe(pipex);
}

/* Calling pipe() for each cmd 
void	ft_create_pipes(int pipes[1024][2])
{
	int	i;

	i = -1;
	while (++i < pipex->cmd_nmbs - 1)
		if (pipe(pipes[i]) == -1)
			ft_free_parent(pipex);
}
void	ft_close_pipes(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < pipex->pipe_nmbs)
		close(pipex->pipe[i]);
}
*/
