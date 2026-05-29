/* Program Number: 1
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates different file types including soft link
   (symlink system call), hard link (link system call), and FIFO (mkfifo
   library function and mknod system call).
*/

#include <stdio.h>      /* For printf, perror */
#include <stdlib.h>     /* For exit */
#include <unistd.h>     /* For symlink, link, close, write */
#include <sys/types.h>  /* For mode_t and other type definitions */
#include <sys/stat.h>   /* For mkfifo, mknod, S_IFIFO */
#include <fcntl.h>      /* For creat, O_WRONLY */

int main() {
    int fd;

    /* Step 1: First, create a regular file that we will create links to.
       creat() creates a new file or truncates an existing one.
       0644 means: owner can read/write, group and others can only read. */
    fd = creat("original_file.txt", 0644);
    if (fd == -1) {
        perror("creat");  /* Print error message if file creation fails */
        exit(1);
    }
    /* Write some content to the file so we can verify links work */
    write(fd, "Hello, this is the original file.\n", 33);
    close(fd);  /* Always close the file after we're done */
    printf("Original file 'original_file.txt' created successfully.\n");

    /* ------------------------------------------------------------ */
    /* (a) Create a SOFT LINK (symbolic link) using symlink() system call.
       A soft link is like a shortcut or pointer to the original file.
       It stores the PATH to the original file, not the data itself.
       If the original file is deleted, the soft link becomes "dangling"
       (broken) because it points to a path that no longer exists.
       Syntax: symlink(target_path, link_name) */
    if (symlink("original_file.txt", "soft_link.txt") == -1) {
        perror("symlink");
        exit(1);
    }
    printf("(a) Soft link 'soft_link.txt' -> 'original_file.txt' created.\n");

    /* ------------------------------------------------------------ */
    /* (b) Create a HARD LINK using link() system call.
       A hard link is another directory entry pointing to the SAME inode
       (same data on disk). Both names are equally valid.
       Even if the original file is deleted, the hard link still has the data
       because the data is only deleted when ALL hard links are removed.
       Syntax: link(existing_file, new_link_name) */
    if (link("original_file.txt", "hard_link.txt") == -1) {
        perror("link");
        exit(1);
    }
    printf("(b) Hard link 'hard_link.txt' -> 'original_file.txt' created.\n");

    /* ------------------------------------------------------------ */
    /* (c) Create a FIFO (named pipe) using mkfifo() library function.
       A FIFO is a special file used for inter-process communication (IPC).
       One process can write data into the FIFO, and another process can
       read that data out. It works like a pipe but has a name in the
       filesystem so unrelated processes can use it.
       0666 means: read/write permission for everyone. */
    if (mkfifo("my_fifo", 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }
    printf("(c) FIFO 'my_fifo' created using mkfifo() library function.\n");

    /* Also create a FIFO using mknod() system call.
       mknod() is a more general system call that can create special files.
       S_IFIFO tells mknod to create a FIFO type file.
       The last argument (0) is the device number, which is ignored for FIFOs. */
    if (mknod("my_fifo2", S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
        exit(1);
    }
    printf("    FIFO 'my_fifo2' created using mknod() system call.\n");

    /* Verification instructions for the user */
    printf("\n--- Verification ---\n");
    printf("Run 'ls -li' to see all created files:\n");
    printf("  - Soft link will show 'l' as file type and '->' pointing to original\n");
    printf("  - Hard link will have the SAME inode number as original_file.txt\n");
    printf("  - FIFOs will show 'p' as file type\n");

    return 0;
}
