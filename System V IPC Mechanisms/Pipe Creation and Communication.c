/* Program Number: 31
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates a pipe, writes data to the pipe,
   reads data from the pipe, and displays the content on the monitor.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];     /* Array to hold pipe file descriptors */
    char write_msg[] = "Hello from the pipe! This is IPC using pipes.";
    char read_buffer[100];
    ssize_t bytes_read;

    printf("===== Pipe Creation and Communication =====\n\n");

    /* pipe() creates a unidirectional data channel (pipe).
       It fills pipefd with two file descriptors:
       - pipefd[0] = READ  end of the pipe (data comes OUT here)
       - pipefd[1] = WRITE end of the pipe (data goes IN here)
       
       Think of it like a water pipe:
       You pour water in one end (write) and it comes out the other (read). */
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    printf("Pipe created successfully!\n");
    printf("Read end  fd: %d\n", pipefd[0]);
    printf("Write end fd: %d\n\n", pipefd[1]);

    /* Write data to the pipe (through the write end) */
    printf("Writing to pipe: \"%s\"\n", write_msg);
    write(pipefd[1], write_msg, strlen(write_msg));

    /* Close the write end after writing.
       This signals EOF to the reader when all data is consumed. */
    close(pipefd[1]);

    /* Read data from the pipe (through the read end) */
    bytes_read = read(pipefd[0], read_buffer, sizeof(read_buffer) - 1);
    if (bytes_read > 0) {
        read_buffer[bytes_read] = '\0';  /* Null-terminate the string */
        printf("Read from pipe: \"%s\"\n", read_buffer);
        printf("Bytes read: %zd\n", bytes_read);
    }

    /* Close the read end */
    close(pipefd[0]);

    printf("\nPipe communication completed successfully!\n");
    return 0;
}
