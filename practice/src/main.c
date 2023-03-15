/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:26:16 by sbocanci          #+#    #+#             */
/*   Updated: 2023/03/15 12:55:28 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define MAX_CMD_LEN 256
#define MAX_CMDS 10

int parse_cmd(char *cmd, char **args) {
    int nargs = 0;
    char *token = strtok(cmd, " ");
    while (token != NULL) {
        args[nargs++] = token;
        token = strtok(NULL, " ");
    }
    args[nargs] = NULL; // add NULL terminator
    return nargs;
}

void parse_and_exec_cmd(char *cmd, int in_fd, int out_fd) {
    pid_t pid = fork();
    if (pid == 0) { // child process
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        // execute the command
        char *args[MAX_CMDS];
        int nargs = parse_cmd(cmd, args);
        execvp(args[0], args);

        // execvp returns only on error
        perror(args[0]);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: %s <infile> \"<command_1>\" \"<command_2>\" ... \"<command_x>\" <outfile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *infile = argv[1];
    char *outfile = argv[argc - 1];
    int num_cmds = argc - 3;
    char *cmds[MAX_CMDS];
    for (int i = 0; i < num_cmds; i++) {
        cmds[i] = argv[i + 2];
    }

    int prev_pipe[2];
    int in_fd = open(infile, O_RDONLY);
    int out_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    for (int i = 0; i < num_cmds; i++) {
        int curr_pipe[2];
        if (pipe(curr_pipe) < 0) {
            perror("pipe error");
            exit(EXIT_FAILURE);
        }
        parse_and_exec_cmd(cmds[i], in_fd, curr_pipe[1]);
        close(curr_pipe[1]);
        if (i > 0) {
            close(prev_pipe[0]);
        }
        prev_pipe[0] = curr_pipe[0];
        prev_pipe[1] = curr_pipe[1];
        in_fd = curr_pipe[0];
    }

    parse_and_exec_cmd("", in_fd, out_fd);

    // Wait for all child processes to finish
    for (int i = 0; i < num_cmds + 1; i++) {
        wait(NULL);
    }

    close(in_fd);
    close(out_fd);

    return EXIT_SUCCESS;
}
