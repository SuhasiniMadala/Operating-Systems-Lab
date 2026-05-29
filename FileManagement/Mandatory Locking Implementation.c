/* Program Number: 16
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program performs mandatory locking using fcntl().
   Part (a) implements a write lock and part (b) implements a read lock.
   Mandatory locking requires the file to have setgid bit set and
   group execute bit cleared (chmod g+s,g-x file).
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int fd;
    struct flock lock;  /* Structure to describe the lock */

    /* Create a test file */
    fd = open("lock_test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    write(fd, "This is test data for locking.\n", 30);

    /* For mandatory locking to work, the file must have:
       - Set-group-ID bit ON  (chmod g+s)
       - Group execute bit OFF (chmod g-x)
       Enable with: chmod 2644 lock_test.txt */
    printf("Note: For mandatory locking, run: chmod 2644 lock_test.txt\n");
    printf("Also ensure filesystem is mounted with 'mand' option.\n\n");

    /* ============================================ */
    /* (a) Implement a WRITE LOCK (exclusive lock).
       A write lock prevents any other process from reading or writing
       the locked region. Only one write lock can exist at a time. */
    printf("(a) Setting WRITE LOCK (F_WRLCK)...\n");

    /* Configure the flock structure for a write lock */
    lock.l_type = F_WRLCK;     /* Write (exclusive) lock */
    lock.l_whence = SEEK_SET;  /* Offset is relative to start of file */
    lock.l_start = 0;          /* Lock starts at byte 0 (beginning) */
    lock.l_len = 0;            /* Lock entire file (0 = until EOF) */
    lock.l_pid = getpid();     /* PID of the process holding the lock */

    /* F_SETLK tries to set the lock. If it can't (another process has it),
       it returns -1 immediately. Use F_SETLKW to wait (block) instead. */
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl write lock");
    } else {
        printf("  Write lock acquired successfully!\n");
        printf("  Lock type: EXCLUSIVE (no other process can read/write)\n");
        printf("  Lock range: entire file\n");
        printf("  Holding lock for 5 seconds...\n");
        sleep(5);

        /* Release the lock by setting type to F_UNLCK */
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        printf("  Write lock released.\n\n");
    }

    /* ============================================ */
    /* (b) Implement a READ LOCK (shared lock).
       A read lock allows other processes to also read (shared access),
       but prevents any process from writing to the locked region.
       Multiple read locks can coexist on the same region. */
    printf("(b) Setting READ LOCK (F_RDLCK)...\n");

    lock.l_type = F_RDLCK;     /* Read (shared) lock */
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;            /* Lock entire file */

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl read lock");
    } else {
        printf("  Read lock acquired successfully!\n");
        printf("  Lock type: SHARED (other processes can also read)\n");
        printf("  Holding lock for 5 seconds...\n");
        sleep(5);

        /* Release the lock */
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        printf("  Read lock released.\n");
    }

    close(fd);
    printf("\nDone. File 'lock_test.txt' closed.\n");
    return 0;
}
