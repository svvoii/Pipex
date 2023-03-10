/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/10 15:58:33 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_free_pipe(t_pipex *pipex)
{
	close(pipex->infile);
	close(pipex->outfile);
	if (pipex->here_doc)
		unlink(".heredoc_tmp");
	free(pipex->pipe);
	ft_msg(ERR_ENVP);
	exit(1);
}

void	ft_free_parent(t_pipex *pipex)
{
	int	i;

	close(pipex->infile);
	close(pipex->outfile);
	if (pipex->here_doc)
		unlink(".heredoc_tmp");
	i = -1;
	while (pipex->cmd_paths[++i])
		free(pipex->cmd_paths[i]);
	free(pipex->cmd_paths);
	free(pipex->pipe);
}

void	ft_free_child(t_pipex *pipex)
{
	int	i;

	i = -1;
	while (pipex->cmd_args[i])
		free(pipex->cmd_args[i]);
	free(pipex->cmd_args);
	free(pipex->cmd);
}
