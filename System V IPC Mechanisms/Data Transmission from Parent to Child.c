/* Program Number: 32
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program sends data from the parent process to the
   child process using a pipe.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char message[] = "Hello child! This message is from your parent via pipe.";
    char buffer[100];

    printf("===== Parent to Child Communication via Pipe =====\n\n");

    /* Create pipe BEFORE fork().
       Both parent and child will inherit the pipe file descriptors.
       Parent will use the write end, child will use the read end. */
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) {
        /* PARENT process: writes to pipe */
        close(pipefd[0]);  /* Close unused READ end in parent */

        printf("Parent (PID=%d): Sending message to child...\n", getpid());
        write(pipefd[1], message, strlen(message));
        printf("Parent: Message sent: \"%s\"\n", message);

        close(pipefd[1]);  /* Close write end after sending */
        wait(NULL);        /* Wait for child to finish */
        printf("\nParent: Child has received the message. Done!\n");
    } else {
        /* CHILD process: reads from pipe */
        close(pipefd[1]);  /* Close unused WRITE end in child */

        printf("Child  (PID=%d): Waiting to read from pipe...\n", getpid());
        ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Child: Received message: \"%s\"\n", buffer);
        }

        close(pipefd[0]);  /* Close read end after reading */
        exit(0);
    }

    return 0;
}
