/* Program Number: 11
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program opens a file, duplicates the file descriptor
   using dup, dup2, and fcntl, appends data with both descriptors,
   and verifies the file is updated correctly.
*/

#include <stdio.h>      /* For printf, perror */
#include <stdlib.h>     /* For exit */
#include <fcntl.h>      /* For open, fcntl, F_DUPFD, O_WRONLY, O_APPEND, O_CREAT */
#include <unistd.h>     /* For dup, dup2, write, close */
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>     /* For strlen */

int main() {
    int fd, fd_dup, fd_dup2, fd_fcntl;
    char buffer[100];
    ssize_t n;

    /* Create and open a file in write mode with append flag.
       O_APPEND makes every write go to the END of the file,
       regardless of where the file pointer currently is. */
    fd = open("dup_test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    printf("Original file descriptor: fd = %d\n\n", fd);

    /* Write initial data using original fd */
    write(fd, "Original FD write.\n", 19);

    /* ============================================================ */
    /* (a) Using dup() to duplicate the file descriptor.
       dup() creates a COPY of the file descriptor.
       The new fd shares the same file table entry as the original,
       meaning they share the same file offset and file status flags.
       dup() returns the LOWEST available file descriptor number. */
    printf("(a) Duplicating fd using dup()...\n");
    fd_dup = dup(fd);
    if (fd_dup == -1) {
        perror("dup");
        exit(1);
    }
    printf("    dup() returned new fd = %d\n", fd_dup);

    /* Write using the duplicated fd. Since both fds share the same
       file table entry, writes from either fd update the same offset. */
    write(fd_dup, "Written via dup() fd.\n", 21);
    printf("    Wrote data using dup'd fd.\n\n");

    /* ============================================================ */
    /* (b) Using dup2() to duplicate the file descriptor.
       dup2(oldfd, newfd) duplicates oldfd to the specified newfd.
       If newfd is already open, it is closed first.
       Unlike dup(), we get to CHOOSE which fd number to use. */
    printf("(b) Duplicating fd using dup2()...\n");
    fd_dup2 = dup2(fd, 10);  /* Duplicate fd to file descriptor 10 */
    if (fd_dup2 == -1) {
        perror("dup2");
        exit(1);
    }
    printf("    dup2() duplicated fd to fd = %d\n", fd_dup2);

    /* Write using the dup2'd fd */
    write(fd_dup2, "Written via dup2() fd.\n", 22);
    printf("    Wrote data using dup2'd fd.\n\n");

    /* ============================================================ */
    /* (c) Using fcntl() with F_DUPFD to duplicate the file descriptor.
       fcntl(fd, F_DUPFD, minfd) duplicates fd to the lowest available
       fd that is >= minfd. This gives us more control than dup().
       F_DUPFD = "duplicate file descriptor" command for fcntl. */
    printf("(c) Duplicating fd using fcntl(F_DUPFD)...\n");
    fd_fcntl = fcntl(fd, F_DUPFD, 20);  /* Get lowest fd >= 20 */
    if (fd_fcntl == -1) {
        perror("fcntl F_DUPFD");
        exit(1);
    }
    printf("    fcntl(F_DUPFD, 20) returned fd = %d\n", fd_fcntl);

    /* Write using the fcntl-duplicated fd */
    write(fd_fcntl, "Written via fcntl() fd.\n", 23);
    printf("    Wrote data using fcntl'd fd.\n\n");

    /* Write one more time with original fd to verify all writes are sequential */
    write(fd, "Final write with original fd.\n", 29);

    /* Close all file descriptors */
    close(fd);
    close(fd_dup);
    close(fd_dup2);
    close(fd_fcntl);

    /* Verification: Read and display the file contents */
    printf("--- Verification: Contents of 'dup_test.txt' ---\n");
    int fd_read = open("dup_test.txt", O_RDONLY);
    if (fd_read != -1) {
        while ((n = read(fd_read, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[n] = '\0';
            printf("%s", buffer);
        }
        close(fd_read);
    }

    printf("\n--- All writes appeared sequentially because all fds share\n");
    printf("    the same file table entry and file offset. ---\n");

    return 0;
}
