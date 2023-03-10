/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_child.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/10 16:24:00 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_child(t_pipex p, char **av, char **envp);
char	*ft_get_cmd(char **paths, char *cmd);
void	ft_sub_dup2(int zero, int first);
int		ft_msg(char *err);
void	ft_msg_pipe(char *arg);
void	ft_error(char *err);

void	ft_child(t_pipex p, char **av, char **envp)
{
	p.pid = fork();
	if (p.pid == 0)
	{
		if (p.idx == 0)
			ft_sub_dup2(p.infile, p.pipe[1]);
		else if (p.idx == p.cmd_nmbs - 1)
			ft_sub_dup2(p.pipe[2 * p.idx - 2], p.outfile);
		else
			ft_sub_dup2(p.pipe[2 * p.idx - 2], p.pipe[2 * p.idx + 1]);
		close_pipes(&p);
		p.cmd_args = ft_split(av[2 + p.here_doc + p.idx], ' ');
		p.cmd = ft_get_cmd(p.cmd_paths, p.cmd_args[0]);
		if (!p.cmd)
		{
			ft_msg_pipe(p.cmd_args[0]);
			child_free(&p);
			exit(1);
		}
		execve(p.cmd, p.cmd_args, envp);
	}
}

char	*ft_get_cmd(char **paths, char *cmd)
{
	char	*command;
	char	*tmp;

	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, 0) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

void	ft_sub_dup2(int zero, int first)
{
	dup2(zero, 0);
	dup2(first, 1);
}

int	ft_msg(char *err)
{
	write(2, err, ft_strlen(err));
	return (1);
}

void	ft_msg_pipe(char *arg)
{
	write(2, ERR_CMD, ft_strlen(ERR_CMD));
	write(2, arg, ft_strlen(arg));
	write(2, "\n", 1);
}

void	ft_error(char *err)
{
	perror(err);
	exit(1);
}
