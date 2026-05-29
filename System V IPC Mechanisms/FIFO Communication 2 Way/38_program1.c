/* Program Number: 38 (Program 1)
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: Program 1 for two-way FIFO communication.
   Uses two FIFOs: writes to fifo1, reads from fifo2.
   Run both programs in separate terminals.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define FIFO1 "fifo1"  /* Program1 writes, Program2 reads */
#define FIFO2 "fifo2"  /* Program2 writes, Program1 reads */

int main() {
    int fd_write, fd_read;
    char message[100], buffer[100];

    /* Create both FIFOs */
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    printf("===== Two-Way FIFO: Program 1 =====\n");
    printf("Waiting for Program 2 to connect...\n");

    /* Open FIFO1 for writing and FIFO2 for reading */
    fd_write = open(FIFO1, O_WRONLY);
    fd_read = open(FIFO2, O_RDONLY);
    printf("Connected! Start chatting (type 'quit' to exit):\n\n");

    while (1) {
        /* Send a message */
        printf("Program1> ");
        fgets(message, sizeof(message), stdin);
        write(fd_write, message, strlen(message));

        if (strncmp(message, "quit", 4) == 0) break;

        /* Receive a reply */
        ssize_t n = read(fd_read, buffer, sizeof(buffer) - 1);
        if (n <= 0) { printf("Connection closed.\n"); break; }
        buffer[n] = '\0';

        if (strncmp(buffer, "quit", 4) == 0) {
            printf("Program 2 quit.\n"); break;
        }
        printf("Program2: %s", buffer);
    }

    close(fd_write);
    close(fd_read);
    unlink(FIFO1);
    unlink(FIFO2);
    return 0;
}
