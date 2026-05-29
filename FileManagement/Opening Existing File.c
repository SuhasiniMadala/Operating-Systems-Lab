/* Program Number: 4
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program opens an existing file in read-write mode
   and experiments with the O_EXCL flag to understand its behavior.
*/

#include <stdio.h>      /* For printf, perror */
#include <stdlib.h>     /* For exit */
#include <fcntl.h>      /* For open, O_RDWR, O_CREAT, O_EXCL */
#include <unistd.h>     /* For close, write, read */
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>     /* For strlen */

int main() {
    int fd;
    char buffer[100];
    ssize_t bytes_read;

    /* First, let's create a file so we have an "existing file" to work with */
    fd = creat("existing_file.txt", 0644);
    if (fd == -1) {
        perror("creat");
        exit(1);
    }
    write(fd, "This file already exists.\n", 25);
    close(fd);
    printf("Step 1: Created 'existing_file.txt' for testing.\n\n");

    /* ------------------------------------------------------------ */
    /* Open the existing file in read-write mode (O_RDWR).
       O_RDWR means we can both read from AND write to the file.
       This should succeed because the file exists. */
    fd = open("existing_file.txt", O_RDWR);
    if (fd == -1) {
        perror("open O_RDWR");
        exit(1);
    }
    printf("Step 2: Opened 'existing_file.txt' in O_RDWR mode. fd = %d\n", fd);

    /* Read the content to verify */
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  /* Null-terminate the string */
        printf("Content read: %s\n", buffer);
    }
    close(fd);

    /* ------------------------------------------------------------ */
    /* Experiment with O_EXCL flag:
       O_EXCL is used together with O_CREAT. When both are specified:
       - If the file DOES NOT exist: open() creates it and succeeds.
       - If the file ALREADY exists: open() FAILS with errno = EEXIST.

       O_EXCL is useful to ensure that you are creating a NEW file and
       not accidentally opening an existing one. It acts as a safety check.

       This is commonly used for lock files - you want to make sure
       only one process creates the lock file. */

    printf("Step 3: Trying to open existing file with O_CREAT | O_EXCL...\n");
    fd = open("existing_file.txt", O_RDWR | O_CREAT | O_EXCL, 0644);
    if (fd == -1) {
        /* This is EXPECTED to fail because the file already exists */
        perror("open with O_EXCL (expected to fail)");
        printf("  -> O_EXCL correctly prevented opening an existing file!\n\n");
    } else {
        printf("  -> File opened (this shouldn't happen for existing file). fd = %d\n", fd);
        close(fd);
    }

    /* ------------------------------------------------------------ */
    /* Now try O_EXCL with a NEW file that doesn't exist yet.
       This should succeed because the file doesn't exist. */
    printf("Step 4: Trying to create a new file with O_CREAT | O_EXCL...\n");
    fd = open("brand_new_file.txt", O_RDWR | O_CREAT | O_EXCL, 0644);
    if (fd == -1) {
        perror("open new file with O_EXCL");
    } else {
        printf("  -> Successfully created 'brand_new_file.txt'. fd = %d\n", fd);
        write(fd, "This is a brand new file.\n", 25);
        close(fd);
    }

    printf("\n--- Summary ---\n");
    printf("O_RDWR: Opens file for both reading and writing.\n");
    printf("O_EXCL + O_CREAT: Fails if file exists, creates if it doesn't.\n");
    printf("This combination ensures atomic file creation (no race conditions).\n");

    return 0;
}
