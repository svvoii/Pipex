/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/20 12:25:23 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_multiple_pipes(int ac, char **av, char **env);
void	ft_pipe(t_pipex *current, int ac, int i, char *file);
t_pipex	*ft_init(int pos, char *file);
void	ft_free_struc(t_pipex *struc);

int	main(int ac, char **av, char **envp)
{
	int	fd[2];

	if (ac < 5 || (!ft_strncmp(av[1], "here_doc", ft_strlen(av[1])) && ac > 6))
	{
		write(2, "Error: Invalid arguments\n", 25);
		write(2, "Use: ./pipex <file1> <cmd1> <cmd2> .. <file2>\n", 46);
		return (1);
	}
	if (ac == 6 && (!ft_strncmp(av[1], "here_doc", ft_strlen(av[1]))))
	{
		ft_here_doc(av);
		pipe(fd);
		ft_execve(av, envp, 0, fd);
		close(fd[1]);
		ft_execve(av, envp, 1, fd);
		close(fd[0]);
		while (wait(0) > 0)
		{
		}
	}
	else
		ft_multiple_pipes(ac, av, envp);
	return (0);
}

void	ft_multiple_pipes(int ac, char **av, char **env)
{
	t_pipex	*current;
	t_pipex	*previous;
	int		i;

	i = 1;
	current = ft_init(2, av[i]);
	while (++i < ac - 1)
	{
		current->cmd = av[i];
		if (i != ac - 2)
			ft_pipe(current, ac - 1, i, av[ac - 1]);
		ft_exec_cmd(current, av, env, ac);
		if (current->wr_out != 1)
			close(current->wr_out);
		if (current->rd_in != 0)
			close(current->rd_in);
		previous = current;
		current = current->next;
		ft_free_struc(previous);
	}
	while (wait(0) > 0)
	{
	}
}

void	ft_pipe(t_pipex *current, int ac, int i, char *file)
{
	int	fd[2];

	if (i + 2 == ac)
		current->next = ft_init(i + 1, file);
	else
		current->next = ft_init(i + 1, "");
	pipe(fd);
	current->wr_out = fd[1];
	current->next->rd_in = fd[0];
}

t_pipex	*ft_init(int pos, char *file)
{
	t_pipex	*tmp;

	tmp = (t_pipex *)malloc(sizeof(t_pipex));
	tmp->rd_in = 0;
	tmp->file_name = file;
	tmp->pos = pos;
	tmp->wr_out = 1;
	return (tmp);
}

void	ft_free_struc(t_pipex *struc)
{
	if (struc)
	{
		free(struc);
		struc = NULL;
	}
}
