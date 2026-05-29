/* Program Number: 14
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program takes a filename from the command line
   and identifies the type of file (regular, directory, symlink, etc.)
   using the lstat() system call and S_IS* macros.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct stat file_info;

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    /* lstat() is used instead of stat() because lstat does NOT follow
       symbolic links. stat() would give info about the target file,
       but lstat() gives info about the link itself. */
    if (lstat(argv[1], &file_info) == -1) {
        perror("lstat");
        exit(1);
    }

    printf("File: %s\n", argv[1]);
    printf("Type: ");

    /* S_ISREG, S_ISDIR, etc. are macros that check the file type
       bits in st_mode. st_mode contains both file type and permissions. */
    if (S_ISREG(file_info.st_mode))
        printf("Regular File\n");
    else if (S_ISDIR(file_info.st_mode))
        printf("Directory\n");
    else if (S_ISLNK(file_info.st_mode))
        printf("Symbolic Link\n");
    else if (S_ISFIFO(file_info.st_mode))
        printf("FIFO (Named Pipe)\n");
    else if (S_ISCHR(file_info.st_mode))
        printf("Character Device\n");
    else if (S_ISBLK(file_info.st_mode))
        printf("Block Device\n");
    else if (S_ISSOCK(file_info.st_mode))
        printf("Socket\n");
    else
        printf("Unknown File Type\n");

    /* Also print the permission bits in octal format */
    printf("Permissions: %o\n", file_info.st_mode & 0777);
    printf("Size: %ld bytes\n", (long)file_info.st_size);

    return 0;
}
