/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_forks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 10:56:48 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/08 15:34:02 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROCESSES 7

int	main(int ac, char **av)
{
	int	process_ids[PROCESSES];
	int	pipes[PROCESSES + 1][2];
	int	i, j, x, y;

	i = -1;
	while (++i < PROCESSES + 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			printf("Error creating pipe '%d'\n", i);
			return (1);
		}
	}
	i = -1;
	while (++i < PROCESSES)
	{
		process_ids[i] = fork();
		if (process_ids[i] == -1)
		{
			printf("Error creating process '%d'\n", i);
			return (2);
		}
		if (process_ids[i] == 0)
		{
			// Child process
			j = -1;
			while (++j < PROCESSES + 1)
			{
				if (i != j)
					close(pipes[j][0]);
				if (i + 1 != j)
					close(pipes[j][1]);
			}
			x = 0;
			if (read(pipes[i][0], &x, sizeof(int)) == -1)
			{
				//printf("\tError at reading '%d'\n", i);
				printf("\tError at reading\n");
				return (3);
			}
			printf("\t(%d) got '%d'\n", i, x);
			x++;
			if (write(pipes[i + 1][1], &x, sizeof(int)) == -1)
			{
				//printf("\tError at writing '%d' (i + 1)\n", i + 1);
				printf("\tError at writing\n");
				return (4);
			}
			printf("\t(%d) sent '%d'\n", i, x);
			printf("\n");
			close(pipes[i][0]);
			close(pipes[i + 1][1]);
			return (0);
		}
	}

	// Main process
	j = -1;
	while (++j != PROCESSES)
	{
		if (j != PROCESSES)
			close(pipes[j][0]);
		if (j != 0)
			close(pipes[j][1]);
	}
	y = 0;
	printf("Main process sent '%d'\n", y);
	if (write(pipes[0][1], &y, sizeof(int)) == -1)
	{
		printf("Error writing in main process\n");
		return (4);
	}
	if (read(pipes[PROCESSES][0], &y, sizeof(int)) == -1)
	{
		printf("Error reading in main process\n");
		return (3);
	}
	printf("The final result is '%d'\n", y);
	close(pipes[0][1]);
	close(pipes[PROCESSES][0]);

	i = -1;
	while (++i < PROCESSES)
	{
		wait(NULL);
	}

	return (0);
}
