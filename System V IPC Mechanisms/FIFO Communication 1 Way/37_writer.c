/* Program Number: 37 (Writer)
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This is the WRITER program for one-way FIFO communication.
   It writes data to a FIFO file. Run the reader (37_reader.c) first.
   Create FIFO with: mkfifo myfifo
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define FIFO_PATH "myfifo"

int main() {
    int fd;
    char message[100];

    /* Create the FIFO if it doesn't exist.
       mkfifo creates a named pipe in the filesystem. */
    mkfifo(FIFO_PATH, 0666);

    printf("===== FIFO Writer (One-Way) =====\n");
    printf("Waiting for reader to connect...\n");

    /* Open FIFO for writing. This will BLOCK until a reader opens
       the other end of the FIFO. */
    fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open FIFO");
        exit(1);
    }
    printf("Reader connected! Type messages (type 'quit' to exit):\n\n");

    /* Send messages through the FIFO */
    while (1) {
        printf("Writer> ");
        fgets(message, sizeof(message), stdin);

        /* Write the message to the FIFO */
        write(fd, message, strlen(message));

        /* Check if user wants to quit */
        if (strncmp(message, "quit", 4) == 0)
            break;
    }

    close(fd);
    printf("Writer closed.\n");
    return 0;
}
