/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:28:21 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/20 12:23:14 by sbocanci         ###   ########.fr       */
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
	char			*file_name;
	char			*cmd;
	int				rd_in;
	int				wr_out;
	int				pos;
	struct s_pipex	*next;
}	t_pipex;

/* ft_pipex.c (main) */
void	ft_multiple_pipes(int ac, char **av, char **env);
void	ft_pipe(t_pipex *current, int ac, int i, char *file);
t_pipex	*ft_init(int pos, char *file);
void	ft_free_struc(t_pipex *struc);
/* ft_2cmd_exec.c */
void	ft_execve(char **av, char **env, int order, int fd[2]);
void	ft_path(char *av, char **path, char **env);
char	*ft_check_bin(char *cmd, char *command);
char	**ft_set_arr(char *path, char *av);
void	ft_free(char **arr);
/* ft_2cmd_helpers.c*/
void	ft_here_doc(char **av);
void	ft_few_child_ex(char **av, char **arr, int fd[2], int order);
int		ft_second_cmd(int fd[2], char *file);
int		ft_first_cmd(int fd[2]);
void	ft_error_norm(int fd, char *path, char *command);
/* ft_multiple_exec.c*/
void	ft_multiple_child_ex(t_pipex *current, char **av, char **arr, int ac);
int		ft_first(t_pipex *current);
int		ft_last(t_pipex *current);
void	ft_exec_cmd(t_pipex *current, char **av, char **env, int ac);
int		ft_error(char *path, char *command);

#endif