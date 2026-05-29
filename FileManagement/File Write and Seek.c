/* Program Number: 10
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program opens a file in read-write mode, writes 10 bytes,
   moves the file pointer by 10 bytes using lseek, writes another 10 bytes,
   and examines the resulting file with the hole in between.
*/

#include <stdio.h>      /* For printf, perror */
#include <stdlib.h>     /* For exit */
#include <fcntl.h>      /* For open, O_RDWR, O_CREAT, O_TRUNC */
#include <unistd.h>     /* For write, lseek, close */
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int fd;
    off_t offset;

    /* Open (or create) a file in read-write mode.
       O_RDWR  = read and write access
       O_CREAT = create the file if it doesn't exist
       O_TRUNC = truncate (empty) the file if it already exists
       0644    = permissions rw-r--r-- */
    fd = open("lseek_test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    printf("File 'lseek_test.txt' opened. fd = %d\n\n", fd);

    /* Step 1: Write 10 bytes to the file.
       These are the characters "AAAAAAAAAA" (10 A's).
       After this write, the file pointer is at position 10. */
    printf("Step 1: Writing 10 bytes ('AAAAAAAAAA')...\n");
    if (write(fd, "AAAAAAAAAA", 10) != 10) {
        perror("write 1");
        exit(1);
    }
    printf("  Written 10 bytes. File pointer is now at position 10.\n\n");

    /* Step 2: Move the file pointer forward by 10 bytes using lseek().
       lseek() repositions the file offset (the current read/write position).

       Arguments:
       - fd: file descriptor
       - 10: offset (number of bytes to move)
       - SEEK_CUR: move relative to CURRENT position

       SEEK_SET = move relative to the BEGINNING of the file
       SEEK_CUR = move relative to the CURRENT position
       SEEK_END = move relative to the END of the file

       Moving past the end of data creates a "hole" in the file.
       This hole will be filled with null bytes (\0). */
    printf("Step 2: Moving file pointer by 10 bytes using lseek()...\n");
    offset = lseek(fd, 10, SEEK_CUR);

    /* (a) Check the return value of lseek.
       lseek returns the new file offset (from beginning), or -1 on error.
       Since we wrote 10 bytes and then moved 10 more, we should be at 20. */
    if (offset == -1) {
        perror("lseek");
        exit(1);
    }
    printf("  lseek return value (new offset): %ld\n", (long)offset);
    printf("  File pointer is now at position %ld.\n\n", (long)offset);

    /* Step 3: Write another 10 bytes.
       These are "BBBBBBBBBB" (10 B's).
       The gap between positions 10-19 will contain null bytes (\0). */
    printf("Step 3: Writing another 10 bytes ('BBBBBBBBBB')...\n");
    if (write(fd, "BBBBBBBBBB", 10) != 10) {
        perror("write 2");
        exit(1);
    }
    printf("  Written 10 more bytes. File pointer is now at position 30.\n\n");

    /* Close the file */
    close(fd);
    printf("File closed.\n\n");

    /* (b) Instructions to examine the file with od command.
       The 'od' command (octal dump) displays file contents in various formats.
       It will show the null bytes in the hole between the two writes. */
    printf("--- Verification ---\n");
    printf("Run the following commands to examine the file:\n\n");
    printf("  od -c lseek_test.txt     (shows characters, \\0 for null bytes)\n");
    printf("  od -x lseek_test.txt     (shows hexadecimal values)\n");
    printf("  ls -l lseek_test.txt     (shows file size = 30 bytes)\n\n");
    printf("Expected output of 'od -c':\n");
    printf("  Positions  0-9:  A A A A A A A A A A\n");
    printf("  Positions 10-19: \\0 \\0 \\0 \\0 \\0 \\0 \\0 \\0 \\0 \\0  (the hole)\n");
    printf("  Positions 20-29: B B B B B B B B B B\n");
    printf("\nThe \\0 (null bytes) in the middle are the 'hole' created by lseek.\n");
    printf("These bytes are not actually stored on disk (sparse file optimization).\n");

    return 0;
}
