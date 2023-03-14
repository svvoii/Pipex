/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/14 15:49:04 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 100
#define MAX_COMMAND_LENGTH 1000

int main(int argc, char *argv[]) {
    char *infile, *outfile, *command;
    int fd[2], prev_read_fd = STDIN_FILENO, i;
    pid_t pid;
    char *args[MAX_ARGS];
	int prev_pipe[2], curr_pipe[2];

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <infile> \"<command_1>\" \"<command_2>\" ... \"<command_x>\" <outfile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    infile = argv[1];
    outfile = argv[argc-1];
	
	for (int i = 0; i < num_cmds; i++) {
		// Create new pipe
		if (pipe(curr_pipe) < 0) {
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		// Fork new child process
		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) { // child process
			// Set up input redirection
			if (i > 0) {
				if (dup2(prev_pipe[0], STDIN_FILENO) < 0) {
					perror("dup2 error");
					exit(EXIT_FAILURE);
				}
				close(prev_pipe[0]);
			}
			// Set up output redirection
			if (i < num_cmds - 1) {
				if (dup2(curr_pipe[1], STDOUT_FILENO) < 0) {
					perror("dup2 error");
					exit(EXIT_FAILURE);
				}
				close(curr_pipe[1]);
			} else {
				// Last command in pipeline, redirect output to file
				int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
				if (dup2(fd, STDOUT_FILENO) < 0) {
					perror("dup2 error");
					exit(EXIT_FAILURE);
				}
				close(fd);
			}
			// Close unused file descriptors
			close(curr_pipe[0]);
			close(prev_pipe[1]);
			// Parse and execute command
			parse_and_exec_cmd(cmds[i]);
			exit(EXIT_SUCCESS);
		} else { // parent process
			// Close write end of previous pipe
			if (i > 0) {
				close(prev_pipe[1]);
			}
			// Keep read end of current pipe for next command
			prev_pipe[0] = curr_pipe[0];
			prev_pipe[1] = curr_pipe[1];
		}
	}
    // wait for child processes to finish
    for (i = 0; i < argc-3; i++) {
        wait(NULL);
    }

    return 0;
}
