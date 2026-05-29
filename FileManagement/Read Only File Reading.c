/* Program Number: 8
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program opens a file in read-only mode, reads it
   line by line, and displays each line. It closes the file upon
   reaching the end of the file (EOF).
*/

#include <stdio.h>      /* For printf, perror */
#include <stdlib.h>     /* For exit */
#include <fcntl.h>      /* For open, O_RDONLY */
#include <unistd.h>     /* For read, write, close, STDOUT_FILENO */
#include <string.h>     /* For memset */

int main(int argc, char *argv[]) {
    int fd;
    char ch;              /* Single character buffer for reading byte by byte */
    char line[1024];      /* Buffer to accumulate characters into a line */
    int line_pos = 0;     /* Current position in the line buffer */
    int line_num = 1;     /* Line number counter */
    ssize_t bytes_read;

    /* Check if filename is provided as command line argument */
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        printf("Example: %s myfile.txt\n", argv[0]);
        exit(1);
    }

    /* Open the file in read-only mode (O_RDONLY).
       This means we can only read from the file, not write to it. */
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        printf("Could not open file '%s'. Make sure it exists.\n", argv[1]);
        exit(1);
    }
    printf("File '%s' opened in read-only mode. fd = %d\n\n", argv[1], fd);
    printf("--- File Contents (line by line) ---\n\n");

    /* Read the file one character at a time.
       We accumulate characters until we find a newline '\n',
       which marks the end of a line. Then we print the complete line. */
    while (1) {
        /* Read one byte (character) at a time from the file */
        bytes_read = read(fd, &ch, 1);

        if (bytes_read == -1) {
            /* Error occurred while reading */
            perror("read");
            close(fd);
            exit(1);
        }

        if (bytes_read == 0) {
            /* End of file (EOF) reached.
               If there's any remaining data in the line buffer
               (last line without newline), print it. */
            if (line_pos > 0) {
                line[line_pos] = '\0';
                printf("Line %d: %s\n", line_num, line);
            }
            break;  /* Exit the loop */
        }

        /* If we encounter a newline character, it marks end of a line */
        if (ch == '\n') {
            line[line_pos] = '\0';  /* Null-terminate the string */
            printf("Line %d: %s\n", line_num, line);
            line_num++;
            line_pos = 0;  /* Reset position for the next line */
        } else {
            /* Add the character to our line buffer */
            if (line_pos < (int)(sizeof(line) - 1)) {
                line[line_pos++] = ch;
            }
        }
    }

    printf("\n--- End of File ---\n");
    printf("Total lines read: %d\n", line_num);

    /* Close the file to release the file descriptor */
    close(fd);
    printf("File closed successfully.\n");

    return 0;
}
