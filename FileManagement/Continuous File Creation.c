/* Program Number: 5
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates five new files in an infinite loop.
   Run it in the background and inspect /proc/pid/fd to see the
   file descriptor table growing.
*/

#include <stdio.h>      /* For printf, perror, snprintf */
#include <stdlib.h>     /* For exit */
#include <fcntl.h>      /* For open, O_CREAT, O_WRONLY */
#include <unistd.h>     /* For close, getpid, sleep */
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int fd;
    char filename[50];
    int count = 0;   /* Counter to generate unique filenames */
    pid_t pid = getpid();  /* Get our process ID */

    printf("Program PID: %d\n", pid);
    printf("Run this in background: ./a.out &\n");
    printf("Then inspect file descriptors: ls -l /proc/%d/fd\n\n", pid);

    /* Infinite loop: keep creating 5 files in each iteration.
       IMPORTANT: We intentionally do NOT close the file descriptors.
       This means the file descriptor table (/proc/pid/fd) will keep
       growing with each new file we open.

       The file descriptor table is a per-process table maintained by
       the kernel that keeps track of all open files. Each entry in
       /proc/<pid>/fd is a symbolic link to the actual file. */

    while (1) {
        int i;
        printf("--- Creating 5 files (batch starting at %d) ---\n", count);

        for (i = 0; i < 5; i++) {
            /* Create a unique filename for each file */
            snprintf(filename, sizeof(filename), "tempfile_%d.txt", count);

            /* Open (create) the file. O_CREAT creates if it doesn't exist.
               O_WRONLY opens for writing only.
               0644 = owner read/write, group/others read only. */
            fd = open(filename, O_CREAT | O_WRONLY, 0644);

            if (fd == -1) {
                perror("open");
                printf("Failed to create file %s. Possibly hit max open files limit.\n", filename);
                printf("Check limit with: ulimit -n\n");
                exit(1);
            }

            printf("Created '%s', file descriptor = %d\n", filename, fd);
            /* NOTE: We are NOT closing the file descriptor intentionally!
               This lets us observe the growing fd table in /proc/pid/fd */

            count++;
        }

        printf("Check /proc/%d/fd now! (ls -l /proc/%d/fd)\n\n", pid, pid);

        /* Sleep for 5 seconds between batches so the user has time
           to inspect the /proc/pid/fd directory */
        sleep(5);
    }

    /* This line will never be reached because of the infinite loop.
       To stop the program, use: kill <pid> or Ctrl+C (if foreground) */
    return 0;
}
