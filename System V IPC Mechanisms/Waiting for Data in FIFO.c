/* Program Number: 39
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program waits for data to be written into a FIFO
   within 10 seconds using the select() system call.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>

#define FIFO_PATH "select_fifo"

int main() {
    int fd;
    fd_set read_fds;
    struct timeval timeout;
    char buffer[256];

    /* Create the FIFO */
    mkfifo(FIFO_PATH, 0666);
    printf("===== Waiting for FIFO Data using select() =====\n\n");
    printf("FIFO '%s' created.\n", FIFO_PATH);
    printf("To send data, run in another terminal:\n");
    printf("  echo 'Hello' > %s\n\n", FIFO_PATH);

    /* Open FIFO in non-blocking mode so open() doesn't block.
       O_RDONLY | O_NONBLOCK allows us to open without waiting for a writer. */
    fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open FIFO");
        exit(1);
    }

    /* Set up select() to wait for data on the FIFO for 10 seconds */
    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting up to 10 seconds for data on FIFO...\n\n");
    int result = select(fd + 1, &read_fds, NULL, NULL, &timeout);

    if (result == -1) {
        perror("select");
    } else if (result == 0) {
        printf("TIMEOUT! No data received within 10 seconds.\n");
    } else {
        if (FD_ISSET(fd, &read_fds)) {
            ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0';
                printf("Data received from FIFO: %s\n", buffer);
            } else {
                printf("FIFO was opened by writer but no data yet.\n");
            }
        }
    }

    close(fd);
    unlink(FIFO_PATH);
    printf("Done.\n");
    return 0;
}
