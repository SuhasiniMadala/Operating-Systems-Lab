/* Program Number: 37 (Reader)
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This is the READER program for one-way FIFO communication.
   It reads data from a FIFO file written by the writer (37_writer.c).
   Run this FIRST, then run the writer in another terminal.
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
    char buffer[100];
    ssize_t bytes_read;

    /* Create the FIFO if it doesn't exist */
    mkfifo(FIFO_PATH, 0666);

    printf("===== FIFO Reader (One-Way) =====\n");
    printf("Waiting for writer to connect...\n");

    /* Open FIFO for reading. This blocks until a writer opens it. */
    fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open FIFO");
        exit(1);
    }
    printf("Writer connected! Waiting for messages...\n\n");

    /* Continuously read messages from the FIFO */
    while (1) {
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            printf("Writer disconnected.\n");
            break;
        }
        buffer[bytes_read] = '\0';

        if (strncmp(buffer, "quit", 4) == 0) {
            printf("Received quit signal. Exiting.\n");
            break;
        }

        printf("Reader received: %s", buffer);
    }

    close(fd);
    unlink(FIFO_PATH);  /* Clean up the FIFO file */
    printf("Reader closed.\n");
    return 0;
}
