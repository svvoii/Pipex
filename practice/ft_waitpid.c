/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_waitpid.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 10:56:48 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/08 13:40:14 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	main(int ac, char **av)
{
	int	pid_1;
	int	pid_2;
	int	pid_1_res;
	int	pid_2_res;

	pid_1 = fork();
	if (pid_1 == -1)
	{
		printf("Error creating process pid_1");
		return (1);
	}
	if (pid_1 == 0)
	{
		sleep(4);
		printf("Finished execution (pid_1) '%d'\n", getpid());
		return (0);
	}
	pid_2 = fork();
	if (pid_2 == -1)	
	{
		printf("Error creating process pid_2");
		return (2);
	}
	if (pid_2 == 0)
	{
		sleep(1);
		printf("Finished execution (pid_2) '%d'\n", getpid());
		return (0);
	}
	
	//pid_1_res = wait(NULL);
	pid_1_res = waitpid(pid_1, NULL, 0);
	printf("Waited for '%d'\n", pid_1_res);	
	//pid_2_res = wait(NULL);
	pid_2_res = waitpid(pid_2, NULL, 0);
	printf("Waited for '%d'\n", pid_2_res);	

	return (0);
}
