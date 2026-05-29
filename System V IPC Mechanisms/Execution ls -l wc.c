/* Program Number: 34
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program executes 'ls -l | wc' using:
   (a) dup, (b) dup2, and (c) fcntl for redirecting pipe fds.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;

    /* The idea: create a pipe, fork, then:
       - Child runs 'ls -l' with stdout redirected to pipe write end
       - Parent runs 'wc' with stdin redirected from pipe read end
       This achieves: ls -l | wc */

    /* ======== (a) Using dup ======== */
    printf("===== (a) ls -l | wc using dup =====\n");
    pipe(pipefd);
    pid = fork();
    if (pid == 0) {
        /* Child: run 'ls -l', redirect stdout to pipe */
        close(pipefd[0]);       /* Close unused read end */
        close(STDOUT_FILENO);   /* Close stdout (fd 1) */
        dup(pipefd[1]);         /* dup gives lowest available fd = 1 (stdout) */
        close(pipefd[1]);       /* Close original pipe write end */
        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls"); exit(1);
    }
    /* Parent: run 'wc', redirect stdin from pipe */
    close(pipefd[1]);       /* Close unused write end */
    close(STDIN_FILENO);    /* Close stdin (fd 0) */
    dup(pipefd[0]);         /* dup gives lowest available fd = 0 (stdin) */
    close(pipefd[0]);       /* Close original pipe read end */
    wait(NULL);
    /* Instead of exec'ing wc in the parent, fork again */
    pid = fork();
    if (pid == 0) {
        execlp("wc", "wc", NULL);
        perror("execlp wc"); exit(1);
    }
    wait(NULL);

    /* Since we modified our own stdin, we need to re-exec for parts b & c.
       For simplicity, we demonstrate all three approaches in separate child groups. */
    printf("\n===== (b) ls -l | wc using dup2 =====\n");
    pipe(pipefd);
    pid = fork();
    if (pid == 0) {
        /* Child 1: ls -l */
        close(pipefd[0]);
        /* dup2(oldfd, newfd) - directly assigns oldfd to newfd.
           No need to close newfd first like with dup. */
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execlp("ls", "ls", "-l", NULL);
        perror("execlp"); exit(1);
    }
    close(pipefd[1]);
    pid_t pid2 = fork();
    if (pid2 == 0) {
        /* Child 2: wc */
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execlp("wc", "wc", NULL);
        perror("execlp"); exit(1);
    }
    close(pipefd[0]);
    wait(NULL); wait(NULL);

    printf("\n===== (c) ls -l | wc using fcntl =====\n");
    pipe(pipefd);
    pid = fork();
    if (pid == 0) {
        /* Child 1: ls -l */
        close(pipefd[0]);
        close(STDOUT_FILENO);
        /* fcntl with F_DUPFD duplicates to lowest fd >= arg.
           Since we closed fd 1 (stdout), F_DUPFD with 0 gives us fd 1. */
        fcntl(pipefd[1], F_DUPFD, 0);
        close(pipefd[1]);
        execlp("ls", "ls", "-l", NULL);
        perror("execlp"); exit(1);
    }
    close(pipefd[1]);
    pid2 = fork();
    if (pid2 == 0) {
        /* Child 2: wc */
        close(STDIN_FILENO);
        fcntl(pipefd[0], F_DUPFD, 0);  /* Gets fd 0 (stdin) */
        close(pipefd[0]);
        execlp("wc", "wc", NULL);
        perror("execlp"); exit(1);
    }
    close(pipefd[0]);
    wait(NULL); wait(NULL);

    return 0;
}
