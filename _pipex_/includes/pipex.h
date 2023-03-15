/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:28:21 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/15 18:15:52 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>

# include "libft.h"

typedef struct s_pipex
{
	char	**cmd_paths;
	char	**cmd_args;
	char	*cmd;
	char	*env_path;
	pid_t	pid;
	//int		pipe[1024][2];
	int		infile;
	int		outfile;
	int		here_doc;
	int		cmd_nmbs;
	int		pipe_nmbs;
	//int		i;
}	t_pipex;

/* ft_pipex.c (main) */
void	ft_malloc_pipes(t_pipex *pipex, int ac);
void	ft_get_envp(t_pipex *pipex, char **envp);
//void	ft_create_pipes(int pipex[1024][2]);
//void	ft_create_pipes(int *pipes[2]);
//void	ft_create_pipes(t_pipex *pipex);
void	ft_close_pipes(t_pipex *pipex);
/* ft_execute.c */
void	ft_get_files(t_pipex *pipex, char **av, char *out);
void	ft_here_doc(char *av, t_pipex *pipex);
//void	ft_child(t_pipex p, char **av, char **envp);
void	ft_child(t_pipex p, char **av, char **envp, int pipes[1024][2], int i);
void	ft_sub_dup2(t_pipex p, int pipes[1024][2], int i);
//void	ft_child(t_pipex p, char **av, char **envp, int i);
//void	ft_sub_dup2(t_pipex p);
//void	ft_sub_dup2(int zero, int first);
char	*ft_get_cmd(char **paths, char *cmd);
/* ft_free.c*/
int		ft_args_in(char *arg, t_pipex *pipex);
void	ft_free_child(t_pipex *pipex);
void	ft_free_parent(t_pipex *pipex);
void	ft_free_pipe(t_pipex *pipex);
void	ft_error(char *err);

#endif