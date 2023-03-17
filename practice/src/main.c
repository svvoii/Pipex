/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/17 14:16:31 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_ARGS 10

int main(int argc, char *argv[]) {
    int input_fd, output_fd;
    int pipefd[2];
    int cmd_count = argc - 3;
    char *cmd[MAX_ARGS];
    int cmd_pos = 0;

    if (argc < 4) {
        printf("Usage: %s input_file cmd1 cmd2 ... cmdN output_file\n", argv[0]);
        return 1;
    }

    input_fd = open(argv[1], O_RDONLY);
    if (input_fd < 0) {
        perror("Error opening input file");
        return 1;
    }

    output_fd = open(argv[argc-1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd < 0) {
        perror("Error opening output file");
        return 1;
    }

    for (int i = 2; i < argc-1; i++) {
        cmd[cmd_pos++] = argv[i];
        if (strcmp(argv[i], "|") == 0 || i == argc-2) {
            cmd[cmd_pos] = NULL;

            if (pipe(pipefd) < 0) {
                perror("Error creating pipe");
                return 1;
            }

            pid_t pid = fork();
            if (pid < 0) {
                perror("Error forking");
                return 1;
            } else if (pid == 0) {
                if (i == argc-2) {
                    dup2(output_fd, STDOUT_FILENO);
                } else if (i == 2) {
                    dup2(input_fd, STDIN_FILENO);
                    dup2(pipefd[1], STDOUT_FILENO);
                } else {
                    dup2(pipefd[0], STDIN_FILENO);
                    dup2(pipefd[1], STDOUT_FILENO);
                }

                close(pipefd[0]);
                close(pipefd[1]);

                execvp(cmd[0], cmd);
                perror("Error executing command");
                exit(1);
            } else {
                close(pipefd[1]);
                input_fd = pipefd[0];
                cmd_pos = 0;
            }
        }
    }

    wait(NULL);
    close(input_fd);
    close(output_fd);

    return 0;
}
