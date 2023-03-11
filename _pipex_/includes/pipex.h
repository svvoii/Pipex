/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sv <sv@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:28:21 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/11 10:36:57 by sv               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/* malloc, free, exit */
# include <stdlib.h>
/* write, read, close, access, pipe, dup, dup2, execve, fork */
# include <unistd.h>
# include <sys/types.h>
/* perror */
# include <stdio.h>
/* strerror */
# include <string.h>
/* open, unlink */
# include <fcntl.h>
/* wait, waitpid */
# include <sys/wait.h>

# include "libft.h"

# define ERR_INFILE "Infile"
# define ERR_OUTFILE "Outfile"
# define ERR_INPUT "Invalid number of arguments.\n"
# define ERR_PIPE "Pipe"
# define ERR_ENVP "Environment"
# define ERR_CMD "Command not found: "
# define ERR_HEREDOC "here_doc"

typedef struct s_pipex
{
	char	**cmd_paths;
	char	**cmd_args;
	char	*cmd;
	char	*env_path;
	pid_t	pid;
	int		*pipe;
	int		infile;
	int		outfile;
	int		here_doc;
	int		cmd_nmbs;
	int		pipe_nmbs;
	int		idx;
}	t_pipex;

/* ft_pipex.c (main) */
void	ft_close_pipes(t_pipex *pipex);
void	ft_create_pipes(t_pipex *pipex);
/* ft_child.c */
void	ft_child(t_pipex p, char **av, char **envp);
char	*ft_get_cmd(char **paths, char *cmd);
void	ft_sub_dup2(int zero, int first);
int		ft_msg(char *err);
void	ft_msg_pipe(char *arg);
void	ft_error(char *err);
/* ft_files.c */
void	ft_get_infile(char **av, t_pipex *pipex);
void	ft_get_outfile(char *av_out, t_pipex *pipex);
char	*ft_find_path(char **envp);
void	ft_here_doc(char *av, t_pipex *pipex);
int		ft_args_in(char *arg, t_pipex *pipex);
/* ft_free.c */
void	ft_free_child(t_pipex *pipex);
void	ft_free_parent(t_pipex *pipex);
void	ft_free_pipe(t_pipex *pipex);

#endif