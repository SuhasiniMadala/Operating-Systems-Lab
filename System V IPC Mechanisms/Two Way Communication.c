/* Program Number: 33
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program implements two-way communication between
   parent and child processes using two pipes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipe1[2];  /* Pipe 1: Parent -> Child */
    int pipe2[2];  /* Pipe 2: Child -> Parent */
    pid_t pid;
    char buffer[100];

    printf("===== Two-Way Pipe Communication =====\n\n");

    /* We need TWO pipes because a single pipe is unidirectional.
       Pipe 1: parent writes, child reads
       Pipe 2: child writes, parent reads */
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid > 0) {
        /* PARENT process */
        close(pipe1[0]);  /* Close read end of pipe1 (parent writes to pipe1) */
        close(pipe2[1]);  /* Close write end of pipe2 (parent reads from pipe2) */

        /* Parent sends message to child via pipe1 */
        char msg[] = "Hello Child! How are you?";
        printf("Parent -> Child: \"%s\"\n", msg);
        write(pipe1[1], msg, strlen(msg));
        close(pipe1[1]);

        /* Parent reads reply from child via pipe2 */
        ssize_t n = read(pipe2[0], buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Parent <- Child: \"%s\"\n", buffer);
        }
        close(pipe2[0]);
        wait(NULL);
        printf("\nTwo-way communication completed!\n");
    } else {
        /* CHILD process */
        close(pipe1[1]);  /* Close write end of pipe1 (child reads from pipe1) */
        close(pipe2[0]);  /* Close read end of pipe2 (child writes to pipe2) */

        /* Child reads message from parent via pipe1 */
        ssize_t n = read(pipe1[0], buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
        }
        close(pipe1[0]);

        /* Child sends reply to parent via pipe2 */
        char reply[] = "Hi Parent! I am doing great!";
        write(pipe2[1], reply, strlen(reply));
        close(pipe2[1]);
        exit(0);
    }

    return 0;
}
