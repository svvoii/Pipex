/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:28:21 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/10 15:06:17 by sbocanci         ###   ########.fr       */
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
}	t_pipex

#endif