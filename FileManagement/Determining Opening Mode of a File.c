/* Program Number: 12
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program finds out the opening mode of a file
   using the fcntl() system call with F_GETFL command.
*/

#include <stdio.h>      /* For printf, perror */
#include <stdlib.h>     /* For exit */
#include <fcntl.h>      /* For open, fcntl, F_GETFL, O_RDONLY, O_WRONLY, etc. */
#include <unistd.h>     /* For close */
#include <sys/types.h>
#include <sys/stat.h>

/* Helper function to print the opening mode of a file descriptor.
   This function uses fcntl() with F_GETFL to get the file status flags
   and then decodes them into human-readable format. */
void print_file_mode(int fd, const char *description) {
    int flags;

    /* fcntl() with F_GETFL returns the file status flags for the fd.
       These flags include the access mode (read/write) and other flags
       like O_APPEND, O_NONBLOCK, etc. that were set when the file was opened. */
    flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return;
    }

    printf("File: %-30s  fd=%d  flags=0x%x  Mode: ", description, fd, flags);

    /* To extract the access mode, we use O_ACCMODE mask.
       O_ACCMODE is a bitmask that isolates the read/write bits.
       The access mode is stored in the lowest two bits of the flags.

       O_RDONLY = 0 (read only)
       O_WRONLY = 1 (write only)
       O_RDWR  = 2 (read and write) */
    int access_mode = flags & O_ACCMODE;

    switch (access_mode) {
        case O_RDONLY:
            printf("READ ONLY (O_RDONLY)");
            break;
        case O_WRONLY:
            printf("WRITE ONLY (O_WRONLY)");
            break;
        case O_RDWR:
            printf("READ-WRITE (O_RDWR)");
            break;
        default:
            printf("UNKNOWN");
            break;
    }

    /* Check for additional flags that may have been set */
    if (flags & O_APPEND)
        printf(" | O_APPEND");    /* Writes always go to end of file */
    if (flags & O_NONBLOCK)
        printf(" | O_NONBLOCK");  /* Non-blocking I/O */
    if (flags & O_SYNC)
        printf(" | O_SYNC");      /* Synchronous writes */

    printf("\n");
}

int main() {
    int fd1, fd2, fd3, fd4;

    /* Create a test file first */
    fd1 = open("mode_test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd1 != -1) {
        write(fd1, "Test file for mode checking.\n", 28);
        close(fd1);
    }

    printf("===== Determining Opening Mode of Files using fcntl() =====\n\n");

    /* Open the same file with different modes and check each one */

    /* Test 1: Open in READ ONLY mode */
    fd1 = open("mode_test.txt", O_RDONLY);
    if (fd1 == -1) { perror("open O_RDONLY"); exit(1); }

    /* Test 2: Open in WRITE ONLY mode */
    fd2 = open("mode_test.txt", O_WRONLY);
    if (fd2 == -1) { perror("open O_WRONLY"); exit(1); }

    /* Test 3: Open in READ-WRITE mode */
    fd3 = open("mode_test.txt", O_RDWR);
    if (fd3 == -1) { perror("open O_RDWR"); exit(1); }

    /* Test 4: Open in WRITE ONLY + APPEND mode */
    fd4 = open("mode_test.txt", O_WRONLY | O_APPEND);
    if (fd4 == -1) { perror("open O_WRONLY|O_APPEND"); exit(1); }

    /* Now use our helper function to determine and print each mode */
    print_file_mode(fd1, "O_RDONLY");
    print_file_mode(fd2, "O_WRONLY");
    print_file_mode(fd3, "O_RDWR");
    print_file_mode(fd4, "O_WRONLY | O_APPEND");

    /* Also check standard file descriptors */
    printf("\n--- Standard File Descriptors ---\n");
    print_file_mode(STDIN_FILENO, "STDIN (fd=0)");
    print_file_mode(STDOUT_FILENO, "STDOUT (fd=1)");
    print_file_mode(STDERR_FILENO, "STDERR (fd=2)");

    /* Close all file descriptors */
    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);

    return 0;
}
