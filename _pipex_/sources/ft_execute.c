/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/15 18:16:18 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_get_files(t_pipex *pipex, char **av, char *out);
void	ft_here_doc(char *av, t_pipex *pipex);
void	ft_child(t_pipex p, char **av, char **envp, int pipes[1024][2], int i);
void	ft_sub_dup2(t_pipex p, int pipes[1024][2], int i);
//void	ft_sub_dup2(int zero, int first);
char	*ft_get_cmd(char **paths, char *cmd);

/* Dealing with in/out files.
** Calling ft_here_doc f() if specified at input */
void	ft_get_files(t_pipex *pipex, char **av, char *out)
{
	if (!ft_strncmp("here_doc", av[1], 9))
		ft_here_doc(av[2], pipex);
	else
	{
		pipex->infile = open(av[1], O_RDONLY);
		if (pipex->infile == -1)
			ft_error(av[1]);
	}
	if (pipex->here_doc)
		pipex->outfile = open(out, O_WRONLY | O_CREAT | O_APPEND, 00007);
	else
		pipex->outfile = open(out, O_CREAT | O_RDWR | O_TRUNC, 00007);
	if (pipex->outfile == -1)
		ft_error(out);
}

/* creates temp hiden .heredoc_tmp document for infile path input. 
** Ptr av here holds the infile (av[2]) */
void	ft_here_doc(char *av, t_pipex *pipex)
{
	char	*buf;
	int		file;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 00007);
	if (file == -1)
		ft_error("here_doc");
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
		ft_error("here_doc");
	}
}

/* Arranging i/o for each pipe and ececuting cmd */
void	ft_child(t_pipex p, char **av, char **envp, int pipes[1024][2], int i)
{
	int	j;
	
	p.pid = fork();
	if (p.pid == 0)
	{
		printf("i:'%d'\tp.cmd:'%s'\n", i, p.cmd);
		ft_sub_dup2(p, pipes, i);
		//ft_close_pipes(&p);
		p.cmd_args = ft_split(av[2 + p.here_doc + i], ' ');
		j = -1;
		while (p.cmd_args[++j])
			printf("cmd_args: '%d': '%s'\n", j, p.cmd_args[j]);
		p.cmd = ft_get_cmd(p.cmd_paths, p.cmd_args[0]);
		printf("cmd: '%s'\n", p.cmd);
		if (!p.cmd)
		{
			write(2, "Error. Command not found: ", 26);
			write(2, p.cmd_args[0], ft_strlen(p.cmd_args[0]));
			write(2, "\n", 1);
			ft_free_child(&p);
			exit(1);
		}
		execve(p.cmd, p.cmd_args, envp);
	}
}

void	ft_sub_dup2(t_pipex p, int pipes[1024][2], int i)
{
	printf("\ti:'%d'\n", i);
	if (i == 0)
	{
		printf("\tif\tdup from:'%d' to '%d' && from:'%d' to '%d'\n", p.infile, STDIN_FILENO, pipes[i][1], STDOUT_FILENO);
		dup2(p.infile, STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
	else if (i == p.cmd_nmbs - 1)
	{
		printf("\telse if\tdup from:'%d' to '%d' && from:'%d' to '%d'\n", pipes[i][0], STDIN_FILENO, p.outfile, STDOUT_FILENO);
		dup2(pipes[i][0], STDIN_FILENO);
		dup2(p.outfile, STDOUT_FILENO);
	}
	else
	{
		printf("\telse\tdup from:'%d' to '%d' && from:'%d' to '%d'\n", pipes[i][0], STDIN_FILENO, pipes[i][1], STDOUT_FILENO);
		dup2(pipes[i][0], STDIN_FILENO);
		dup2(pipes[i][1], STDOUT_FILENO);
	}
}
/*
void	ft_sub_dup2(int read_end, int write_end)
{
	dup2(read_end, STDIN_FILENO);
	dup2(write_end, STDOUT_FILENO);
}
*/
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
