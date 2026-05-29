/* Program Number: 35
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program finds the total number of directories in
   the current directory using: ls -l | grep ^d | wc -l
   Implemented using only dup2 for file descriptor redirection.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe1[2];  /* Pipe between ls and grep */
    int pipe2[2];  /* Pipe between grep and wc */
    pid_t pid;

    printf("===== Count directories using: ls -l | grep ^d | wc -l =====\n\n");

    /* Create both pipes */
    pipe(pipe1);
    pipe(pipe2);

    /* First child: runs 'ls -l'
       stdout -> pipe1 write end */
    pid = fork();
    if (pid == 0) {
        close(pipe1[0]);   /* Close pipe1 read (not needed) */
        close(pipe2[0]);   /* Close pipe2 read (not needed) */
        close(pipe2[1]);   /* Close pipe2 write (not needed) */

        /* Redirect stdout to pipe1 write end using dup2.
           dup2(oldfd, newfd) makes newfd a copy of oldfd. */
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);   /* Close original after dup2 */

        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls"); exit(1);
    }

    /* Second child: runs 'grep ^d'
       stdin <- pipe1 read end, stdout -> pipe2 write end */
    pid = fork();
    if (pid == 0) {
        close(pipe1[1]);   /* Close pipe1 write */
        close(pipe2[0]);   /* Close pipe2 read */

        /* Redirect stdin from pipe1 read end */
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);

        /* Redirect stdout to pipe2 write end */
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[1]);

        execlp("grep", "grep", "^d", NULL);
        perror("execlp grep"); exit(1);
    }

    /* Third child: runs 'wc -l'
       stdin <- pipe2 read end */
    pid = fork();
    if (pid == 0) {
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        /* Redirect stdin from pipe2 read end */
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]);

        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc"); exit(1);
    }

    /* Parent: close all pipe ends and wait for children */
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("(The number above is the count of directories)\n");
    return 0;
}
