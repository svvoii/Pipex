/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_files.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/11 10:24:40 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_get_infile(char **av, t_pipex *pipex);
void	ft_get_outfile(char *av_out, t_pipex *pipex);
char	*ft_find_path(char **envp);

void	ft_here_doc(char *av, t_pipex *pipex);
int		ft_args_in(char *arg, t_pipex *pipex);

void	ft_get_infile(char **av, t_pipex *pipex)
{
	if (!ft_strncmp("here_doc", av[1], 9))
		ft_here_doc(av[2], pipex);
	else
	{
		pipex->infile = open(av[1], O_RDONLY);
		if (pipex->infile == -1)
			ft_error(ERR_INFILE);
	}
}

void	ft_get_outfile(char *av_out, t_pipex *pipex)
{
	if (pipex->here_doc)
		pipex->outfile = open(av_out, O_WRONLY | O_CREAT | O_APPEND, 0000644);
	else
		pipex->outfile = open(av_out, O_CREAT | O_RDWR | O_TRUNC, 0000644);
	if (pipex->outfile == -1)
		ft_error(ERR_OUTFILE);
}

char	*ft_find_path(char **envp)
{
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	return (*envp + 5);
}

void	ft_here_doc(char *av, t_pipex *pipex)
{
	char	*buf;
	int		file;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0000644);
	if (file == -1)
		ft_error(ERR_HEREDOC);
	while (1)
	{
		write(1, "heredoc> ", 9);
		if (get_next_line(0, &buf) < 0)
			exit(1);
		if (!ft_strncmp(av, buf, ft_strlen(av) + 1))
			break ;
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
		free(buf);
	}
	free(buf);
	close(file);
	pipex->infile = open(".heredoc_tmp", O_RDONLY);
	if (pipex->infile == -1)
	{
		unlink(".heredoc_tmp");
		ft_error(ERR_HEREDOC);
	}
}

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