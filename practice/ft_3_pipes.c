/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mult_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 10:56:48 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/08 13:19:19 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(int ac, char **av)
{
	int	fd[3][2];
	int	pid_1;
	int	pid_2;
	int	i;
	int	x, y, z;

	i = -1;
	while (++i < 3)
	{
		if (pipe(fd[i]) < 0)
			return (1);
	}
	// every time we use fork() fd and processes are duplicated
	pid_1 = fork();
	if (pid_1 < 0)
		return (2);

	if (pid_1 == 0)
	{
		// Child process 1
		close(fd[0][1]);
		close(fd[1][0]);
		close(fd[2][0]);
		close(fd[2][1]);
		x = 0;
		if (read(fd[0][0], &x, sizeof(int)) < 0)
			return (3);
		x += 5;
		if (write(fd[1][1], &x, sizeof(int)) < 0)
			return (4);
		close(fd[0][0]);
		close(fd[1][1]);
		return (0);
	}

	pid_2 = fork();
	if (pid_2 < 0)
		return (5);
	if (pid_2 == 0)
	{
		// Child process 2
		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][1]);
		close(fd[2][0]);
		y = 0;
		if (read(fd[1][0], &y, sizeof(int)) < 0)
			return (6);
		y += 5;
		if (write(fd[2][1], &y, sizeof(int)) < 0)
			return (7);
		close(fd[1][0]);
		close(fd[2][1]);
		return (0);
	}

	// Parent process
	close(fd[0][0]);
	close(fd[1][0]);
	close(fd[1][1]);
	close(fd[2][1]);
	z = 0;
	if (write(fd[0][1], &z, sizeof(int)) < 0)
		return (8);
	if (read(fd[2][0], &z, sizeof(int)) < 0)
		return (9);
	printf("result is '%d'\n", z);
	close(fd[0][1]);
	close(fd[2][0]);

	waitpid(pid_1, NULL, 0);
	waitpid(pid_2, NULL, 0);

	return (0);
}
