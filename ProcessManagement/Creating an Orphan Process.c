/* Program Number: 23
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates an orphan process. An orphan is a
   child process whose parent has terminated. The orphan gets adopted
   by the init process (PID 1) or the system's init equivalent.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    printf("===== Creating an Orphan Process =====\n\n");

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        /* CHILD process: stays alive after parent exits.
           When the parent exits first, this child becomes an ORPHAN.
           The operating system then re-parents it to init (PID 1). */
        printf("Child (PID=%d): My parent PID is %d\n", getpid(), getppid());
        printf("Child: Waiting for parent to die (sleeping 5 seconds)...\n");

        sleep(5);  /* Wait for parent to exit */

        /* After the parent exits, getppid() should return 1 (init)
           or the PID of the subreaper process on modern Linux systems. */
        printf("\nChild (PID=%d): My parent has died.\n", getpid());
        printf("Child: My NEW parent PID is %d (should be 1 or init equivalent)\n", getppid());
        printf("Child: I am now an ORPHAN process, adopted by init.\n");
        printf("Child: Exiting now.\n");
        exit(0);
    } else {
        /* PARENT process: exits immediately, orphaning the child. */
        printf("Parent (PID=%d): I created child PID=%d\n", getpid(), pid);
        printf("Parent: I am exiting NOW, making my child an orphan!\n\n");
        /* Parent does NOT wait for child - just exits */
        exit(0);
    }

    return 0;
}
