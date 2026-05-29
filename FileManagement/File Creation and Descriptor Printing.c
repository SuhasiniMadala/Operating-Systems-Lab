/* Program Number: 3
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates a file using the creat() system call
   and prints its file descriptor value.
*/

#include <stdio.h>      /* For printf, perror */
#include <stdlib.h>     /* For exit */
#include <fcntl.h>      /* For creat, O_WRONLY */
#include <unistd.h>     /* For close */
#include <sys/types.h>  /* For mode_t */
#include <sys/stat.h>   /* For file permission macros */

int main() {
    int fd;

    /* creat() system call creates a new file or truncates an existing file.
       It is equivalent to: open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode)

       Arguments:
       - "newfile.txt" : name of the file to create
       - 0644 : file permissions (owner: rw-, group: r--, others: r--)

       creat() returns a file descriptor (a small non-negative integer).
       File descriptors are indices into the kernel's per-process file table.
       - 0 = STDIN  (standard input)
       - 1 = STDOUT (standard output)
       - 2 = STDERR (standard error)
       - 3, 4, 5, ... = files opened by the program

       So the first file we open will typically get fd = 3. */

    fd = creat("newfile.txt", 0644);

    /* Check if creat() failed. It returns -1 on failure. */
    if (fd == -1) {
        perror("creat");  /* perror prints the error message for the last error */
        exit(1);
    }

    /* Print the file descriptor value.
       This should be 3 since 0, 1, 2 are already taken by stdin, stdout, stderr */
    printf("File 'newfile.txt' created successfully.\n");
    printf("File descriptor value: %d\n", fd);
    printf("\nNote: fd=0 is STDIN, fd=1 is STDOUT, fd=2 is STDERR\n");
    printf("So the first user-opened file typically gets fd=3.\n");

    /* Always close the file descriptor when done to free up resources */
    close(fd);
    printf("File closed successfully.\n");

    return 0;
}
