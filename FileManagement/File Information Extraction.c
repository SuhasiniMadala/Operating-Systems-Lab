/* Program Number: 9
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program prints various details about a given file
   including inode, hard links, UID, GID, size, block size, number of
   blocks, and timestamps using the stat() system call.
*/

#include <stdio.h>      /* For printf, perror */
#include <stdlib.h>     /* For exit */
#include <sys/types.h>  /* For various type definitions */
#include <sys/stat.h>   /* For stat() and struct stat */
#include <unistd.h>     /* For stat */
#include <time.h>       /* For ctime() to convert time to readable format */

int main(int argc, char *argv[]) {
    struct stat file_info;  /* Structure to hold file information */

    /* Check if filename is provided */
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    /* stat() system call retrieves information about a file.
       It fills the 'struct stat' with all the file's metadata.
       Unlike lstat(), stat() follows symbolic links (gives info about
       the file the link points to, not the link itself).

       Returns 0 on success, -1 on error. */
    if (stat(argv[1], &file_info) == -1) {
        perror("stat");
        printf("Could not get information about '%s'\n", argv[1]);
        exit(1);
    }

    printf("===== File Information for '%s' =====\n\n", argv[1]);

    /* (a) Inode number: A unique identifier for the file on the filesystem.
       Every file has an inode that stores its metadata (permissions, size, etc.)
       Two files with the same inode number are hard links to the same data. */
    printf("(a) Inode Number       : %lu\n", (unsigned long)file_info.st_ino);

    /* (b) Number of hard links: How many directory entries point to this inode.
       A regular file starts with 1 hard link. Each hard link increases this count.
       A directory typically has at least 2 (itself + '.' entry inside it). */
    printf("(b) Number of Hard Links: %lu\n", (unsigned long)file_info.st_nlink);

    /* (c) UID (User ID): The numeric ID of the file's owner.
       This corresponds to a user in /etc/passwd. */
    printf("(c) UID (Owner User ID): %d\n", file_info.st_uid);

    /* (d) GID (Group ID): The numeric ID of the file's group.
       This corresponds to a group in /etc/group. */
    printf("(d) GID (Owner Group ID): %d\n", file_info.st_gid);

    /* (e) Size: The total size of the file in bytes.
       For a directory, this is the size of the directory metadata,
       not the total size of files inside it. */
    printf("(e) Size               : %ld bytes\n", (long)file_info.st_size);

    /* (f) Block size: The preferred I/O block size for the filesystem.
       This is the optimal size for read/write operations.
       Typically 4096 bytes (4 KB) on modern Linux systems. */
    printf("(f) Block Size         : %ld bytes\n", (long)file_info.st_blksize);

    /* (g) Number of blocks: The number of 512-byte blocks allocated for the file.
       Note: This is in 512-byte blocks, NOT in the block size shown above.
       A small file might use more blocks than expected due to filesystem overhead. */
    printf("(g) Number of Blocks   : %ld (512-byte blocks)\n", (long)file_info.st_blocks);

    /* (h) Time of last access (atime): When the file was last read.
       ctime() converts the time_t value to a human-readable string.
       Note: Some filesystems mount with 'noatime' to improve performance,
       so this might not update on every read. */
    printf("(h) Last Access Time   : %s", ctime(&file_info.st_atime));

    /* (i) Time of last modification (mtime): When the file's CONTENT was last changed.
       This is what 'ls -l' shows by default. */
    printf("(i) Last Modification  : %s", ctime(&file_info.st_mtime));

    /* (j) Time of last change (ctime): When the file's METADATA was last changed.
       This includes changes to permissions, ownership, hard link count, etc.
       Note: This is NOT the creation time! Linux traditionally doesn't track
       creation time (though newer filesystems like ext4 do). */
    printf("(j) Last Status Change : %s", ctime(&file_info.st_ctime));

    /* Bonus: Print the file type */
    printf("\nFile Type              : ");
    if (S_ISREG(file_info.st_mode))       printf("Regular File\n");
    else if (S_ISDIR(file_info.st_mode))  printf("Directory\n");
    else if (S_ISLNK(file_info.st_mode))  printf("Symbolic Link\n");
    else if (S_ISFIFO(file_info.st_mode)) printf("FIFO (Named Pipe)\n");
    else if (S_ISCHR(file_info.st_mode))  printf("Character Device\n");
    else if (S_ISBLK(file_info.st_mode))  printf("Block Device\n");
    else if (S_ISSOCK(file_info.st_mode)) printf("Socket\n");
    else                                   printf("Unknown\n");

    return 0;
}
