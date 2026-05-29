/* Program Number: 38 (Program 2)
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: Program 2 for two-way FIFO communication.
   Uses two FIFOs: reads from fifo1, writes to fifo2.
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
    int fd_read, fd_write;
    char message[100], buffer[100];

    /* Create both FIFOs (in case Program1 hasn't created them yet) */
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    printf("===== Two-Way FIFO: Program 2 =====\n");
    printf("Waiting for Program 1 to connect...\n");

    /* Open FIFO1 for reading and FIFO2 for writing (opposite of Program 1) */
    fd_read = open(FIFO1, O_RDONLY);
    fd_write = open(FIFO2, O_WRONLY);
    printf("Connected! Waiting for messages...\n\n");

    while (1) {
        /* Receive a message first (Program1 sends first) */
        ssize_t n = read(fd_read, buffer, sizeof(buffer) - 1);
        if (n <= 0) { printf("Connection closed.\n"); break; }
        buffer[n] = '\0';

        if (strncmp(buffer, "quit", 4) == 0) {
            printf("Program 1 quit.\n"); break;
        }
        printf("Program1: %s", buffer);

        /* Send a reply */
        printf("Program2> ");
        fgets(message, sizeof(message), stdin);
        write(fd_write, message, strlen(message));

        if (strncmp(message, "quit", 4) == 0) break;
    }

    close(fd_read);
    close(fd_write);
    return 0;
}
