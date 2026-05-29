/* Program Number: 17b
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program opens the ticket file, implements a write lock,
   reads the ticket number, increments it, prints the new ticket number,
   and closes the file. This simulates online ticket reservation with locking.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    struct flock lock;
    char buffer[20];
    int ticket_number;
    ssize_t n;

    /* Open the ticket file in read-write mode */
    fd = open("ticket.txt", O_RDWR);
    if (fd == -1) {
        perror("open ticket.txt (run 17a first!)");
        exit(1);
    }

    /* Set up a WRITE LOCK on the entire file.
       This prevents other processes from reading or writing the ticket
       file while we are modifying it (prevents race conditions). */
    lock.l_type = F_WRLCK;      /* Exclusive write lock */
    lock.l_whence = SEEK_SET;   /* From start of file */
    lock.l_start = 0;           /* Lock from byte 0 */
    lock.l_len = 0;             /* Lock entire file */

    printf("Trying to acquire write lock...\n");

    /* F_SETLKW = set lock and WAIT if another process has the lock.
       This blocks until the lock becomes available. */
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl lock");
        close(fd);
        exit(1);
    }
    printf("Write lock acquired!\n");

    /* Read the current ticket number from the file */
    memset(buffer, 0, sizeof(buffer));
    n = read(fd, buffer, sizeof(buffer) - 1);
    if (n <= 0) {
        printf("Error reading ticket number.\n");
        close(fd);
        exit(1);
    }
    ticket_number = atoi(buffer);
    printf("Current ticket number: %d\n", ticket_number);

    /* Increment the ticket number (simulate issuing a new ticket) */
    ticket_number++;
    printf("New ticket number: %d\n", ticket_number);

    /* Move file pointer back to start to overwrite the old number */
    lseek(fd, 0, SEEK_SET);

    /* Write the new ticket number back to the file */
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%d", ticket_number);
    /* Truncate the file to avoid leftover digits */
    ftruncate(fd, 0);
    lseek(fd, 0, SEEK_SET);
    write(fd, buffer, strlen(buffer));

    /* Simulate some processing time while holding the lock */
    printf("Holding lock for 3 seconds (simulating processing)...\n");
    sleep(3);

    /* Release the lock */
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    printf("Lock released. Ticket booked!\n");

    close(fd);
    return 0;
}
