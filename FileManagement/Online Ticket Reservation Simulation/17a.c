/* Program Number: 17a
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program opens a file, stores a ticket number in it,
   and exits. This is the first part of the ticket reservation simulation.
   Run this program FIRST to initialize the ticket file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    int ticket_number = 100;  /* Initial ticket number */
    char buffer[20];

    /* Create (or overwrite) the ticket file and store the initial ticket number.
       This file will be shared between processes for ticket reservation. */
    fd = open("ticket.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    /* Convert the ticket number to a string and write it to the file */
    snprintf(buffer, sizeof(buffer), "%d", ticket_number);
    write(fd, buffer, strlen(buffer));

    printf("Ticket file 'ticket.txt' created.\n");
    printf("Initial ticket number: %d\n", ticket_number);

    close(fd);
    return 0;
}
