/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 10:56:48 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/08 13:23:45 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(int ac, char **av)
{
	int	fd[2];
	int	pid_1;
	int	pid_2;

	if (pipe(fd) == -1)
		return (1);
	
	pid_1 = fork();
	if (pid_1 < 0)
		return (2);
	
	if (pid_1 == 0)
	{
		// Child process 1 (ping)
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("ping", "ping", "-c", "5", "google.com", NULL);
	}
	
	pid_2 = fork();
	if (pid_2 < 0)
		return (3);
	if (pid_2 == 0)
	{
		// Child process 2 (grep)
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("grep", "grep", "rtt", NULL);
	}

	close(fd[0]);
	close(fd[1]);

	waitpid(pid_1, NULL, 0);
	waitpid(pid_2, NULL, 0);

	return (0);
}
