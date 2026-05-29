/* Program Number: 7
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program copies the contents of file1 into file2,
   emulating the behavior of the $cp file1 file2 command using
   open(), read(), write(), and close() system calls.
*/

#include <stdio.h>      /* For printf, perror, fprintf */
#include <stdlib.h>     /* For exit */
#include <fcntl.h>      /* For open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC */
#include <unistd.h>     /* For read, write, close */
#include <sys/types.h>
#include <sys/stat.h>

/* Buffer size for reading/writing chunks of data */
#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    int fd_src, fd_dest;       /* File descriptors for source and destination */
    ssize_t bytes_read;        /* Number of bytes read in each iteration */
    ssize_t bytes_written;     /* Number of bytes written in each iteration */
    char buffer[BUFFER_SIZE];  /* Buffer to hold data being copied */

    /* Check if the user provided both source and destination filenames.
       argc = argument count, argv = argument values
       argv[0] = program name, argv[1] = source file, argv[2] = destination file */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        fprintf(stderr, "Example: %s file1.txt file2.txt\n", argv[0]);
        exit(1);
    }

    /* Open the source file in read-only mode.
       O_RDONLY = open for reading only.
       We don't need to specify permissions because we're not creating a file. */
    fd_src = open(argv[1], O_RDONLY);
    if (fd_src == -1) {
        perror("open source file");
        exit(1);
    }
    printf("Source file '%s' opened successfully. fd = %d\n", argv[1], fd_src);

    /* Open (or create) the destination file in write-only mode.
       O_WRONLY = open for writing only.
       O_CREAT  = create the file if it doesn't exist.
       O_TRUNC  = if the file exists, truncate it to zero length (erase contents).
       0644     = permissions for newly created file (rw-r--r--). */
    fd_dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dest == -1) {
        perror("open destination file");
        close(fd_src);  /* Close the source file before exiting */
        exit(1);
    }
    printf("Destination file '%s' opened/created. fd = %d\n", argv[2], fd_dest);

    /* Copy loop: read from source, write to destination.
       We read BUFFER_SIZE bytes at a time (or less if the file is small).
       The loop continues until read() returns 0 (end of file). */
    printf("Copying data...\n");

    while ((bytes_read = read(fd_src, buffer, BUFFER_SIZE)) > 0) {
        /* Write the bytes we just read to the destination file.
           We need to make sure ALL bytes are written because write()
           might write fewer bytes than requested. */
        bytes_written = write(fd_dest, buffer, bytes_read);

        if (bytes_written == -1) {
            perror("write");
            close(fd_src);
            close(fd_dest);
            exit(1);
        }

        /* Check if all bytes were written. In rare cases, write() might
           write fewer bytes than requested. */
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Warning: Partial write occurred!\n");
        }
    }

    /* Check if read() failed (returned -1) */
    if (bytes_read == -1) {
        perror("read");
        close(fd_src);
        close(fd_dest);
        exit(1);
    }

    /* Close both files to release resources */
    close(fd_src);
    close(fd_dest);

    printf("File '%s' copied to '%s' successfully!\n", argv[1], argv[2]);

    return 0;
}
