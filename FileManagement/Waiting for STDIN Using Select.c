/* Program Number: 13
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program waits for input from STDIN for 10 seconds
   using the select() system call with proper print statements.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

int main() {
    fd_set read_fds;
    struct timeval timeout;
    int result;
    char buffer[256];
    ssize_t bytes_read;

    printf("===== Waiting for STDIN using select() =====\n\n");
    printf("You have 10 seconds to type something and press Enter.\n\n");

    /* Initialize the fd set - clear it then add STDIN */
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    /* Set timeout to 10 seconds */
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    /* select() monitors file descriptors for readiness.
       Args: nfds (max fd+1), read set, write set, except set, timeout.
       Returns: >0 = ready fds, 0 = timeout, -1 = error */
    printf("Calling select()... waiting up to 10 seconds...\n\n");
    result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);

    if (result == -1) {
        perror("select");
        exit(1);
    } else if (result == 0) {
        printf("*** TIMEOUT! No data entered within 10 seconds. ***\n");
    } else {
        printf("Data is available! select() returned %d\n", result);
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("You typed: %s", buffer);
            }
        }
    }

    printf("Remaining time: %ld.%06ld seconds\n",
           (long)timeout.tv_sec, (long)timeout.tv_usec);
    return 0;
}
