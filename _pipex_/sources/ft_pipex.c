/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/11 10:21:05 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_close_pipes(t_pipex *pipex);
void	ft_create_pipes(t_pipex *pipex);

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	if (ac < ft_args_in(av[1], &pipex))
		return (ft_msg(ERR_INPUT));
	ft_get_infile(av, &pipex);
	ft_get_outfile(av[ac - 1], &pipex);
	pipex.cmd_nmbs = ac - 3 - pipex.here_doc;
	pipex.pipe_nmbs = 2 * (pipex.cmd_nmbs - 1);
	pipex.pipe = (int *)malloc(sizeof(int) * pipex.pipe_nmbs);
	if (!pipex.pipe)
		ft_error(ERR_PIPE);
	pipex.env_path = ft_find_path(envp);
	pipex.cmd_paths = ft_split(pipex.env_path, ':');
	if (!pipex.cmd_paths)
		ft_free_pipe(&pipex);
	ft_create_pipes(&pipex);
	pipex.idx = -1;
	while (++(pipex.idx) < pipex.cmd_nmbs)
		ft_child(pipex, av, envp);
	ft_close_pipes(&pipex);
	waitpid(-1, NULL, 0);
	ft_free_parent(&pipex);
	return (0);
}

void	ft_close_pipes(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < pipex->pipe_nmbs)
		close(pipex->pipe[i]);
}

void	ft_create_pipes(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (++i < pipex->cmd_nmbs - 1)
		if (pipe(pipex->pipe + 2 * i) == -1)
			ft_free_parent(pipex);
}
