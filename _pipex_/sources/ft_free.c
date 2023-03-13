/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/13 16:03:50 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int		ft_args_in(char *arg, t_pipex *pipex);
void	ft_free_child(t_pipex *pipex);
void	ft_free_parent(t_pipex *pipex);
void	ft_free_pipe(t_pipex *pipex);
void	ft_error(char *err);

/* check if there are "here_doc" as and arg in av[1] 
** also check if there are minimum required args */
int	ft_args_in(char *arg, t_pipex *pipex)
{
	if (arg && !ft_strncmp("here_doc", arg, 9))
	{
		pipex->here_doc = 1;
		return (6);
	}
	else
	{
		pipex->here_doc = 0;
		return (5);
	}
}

void	ft_free_pipe(t_pipex *pipex)
{
	close(pipex->infile);
	close(pipex->outfile);
	if (pipex->here_doc)
		unlink(".heredoc_tmp");
	free(pipex->pipe);
	ft_error("Environment");
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

/* Prints out an error passed to it */
void	ft_error(char *err)
{
	perror(err);
	exit(1);
}
