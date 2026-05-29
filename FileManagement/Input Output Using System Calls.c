/* Program Number: 6
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program takes input from STDIN and displays it on
   STDOUT using only read() and write() system calls (not printf/scanf).
*/

#include <stdio.h>      /* For perror */
#include <stdlib.h>     /* For exit */
#include <unistd.h>     /* For read, write, STDIN_FILENO, STDOUT_FILENO */

/* Define buffer size for reading input */
#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    /* File descriptors for standard I/O:
       STDIN_FILENO  = 0 (standard input  - keyboard)
       STDOUT_FILENO = 1 (standard output - screen/terminal)
       STDERR_FILENO = 2 (standard error  - screen/terminal)

       These are defined in <unistd.h> */

    /* First, write a prompt to STDOUT using write() system call.
       write() takes: file_descriptor, buffer, number_of_bytes
       It returns the number of bytes actually written, or -1 on error. */
    const char *prompt = "Enter text (Ctrl+D to end):\n";
    write(STDOUT_FILENO, prompt, 28);

    /* Continuously read from STDIN and write to STDOUT.
       read() takes: file_descriptor, buffer, max_bytes_to_read
       It returns the number of bytes actually read, 0 at end-of-file (EOF),
       or -1 on error.

       When user presses Ctrl+D, STDIN gets EOF and read() returns 0. */
    while (1) {
        /* Read data from standard input (keyboard) into our buffer.
           read() will BLOCK (wait) until the user types something
           and presses Enter. */
        bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

        /* Check for errors */
        if (bytes_read == -1) {
            perror("read");
            exit(1);
        }

        /* Check for end of file (EOF).
           User presses Ctrl+D to signal EOF on the terminal. */
        if (bytes_read == 0) {
            const char *msg = "\nEnd of input (EOF received).\n";
            write(STDOUT_FILENO, msg, 30);
            break;
        }

        /* Echo the read data back to standard output (screen).
           We write exactly the number of bytes we read, no more, no less. */
        const char *echo_msg = "You typed: ";
        write(STDOUT_FILENO, echo_msg, 11);
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    return 0;
}
