/* Program Number: 20
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program calls fork() and prints both the parent
   and child process IDs to demonstrate process creation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    printf("Before fork(): PID = %d\n\n", getpid());

    /* fork() creates a new process by duplicating the calling process.
       After fork(), there are TWO processes running the same code:
       - Parent process: fork() returns the CHILD's PID (positive number)
       - Child process:  fork() returns 0
       - On error:       fork() returns -1

       The child is an exact copy of the parent at the time of forking,
       but they have different PIDs and run independently after fork(). */
    pid = fork();

    if (pid < 0) {
        /* fork() failed - usually due to resource limits */
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        /* This code runs in the CHILD process.
           pid == 0 means we are in the child. */
        printf("=== CHILD PROCESS ===\n");
        printf("  My PID (child):    %d\n", getpid());
        printf("  My Parent PID:     %d\n", getppid());
        printf("  fork() returned:   %d (0 means we are the child)\n", pid);
    } else {
        /* This code runs in the PARENT process.
           pid > 0, and its value is the child's PID. */
        printf("=== PARENT PROCESS ===\n");
        printf("  My PID (parent):   %d\n", getpid());
        printf("  Child's PID:       %d\n", pid);
        printf("  fork() returned:   %d (child's PID)\n", pid);

        /* Wait for child to finish to avoid zombie process */
        wait(NULL);
        printf("\nChild process has finished.\n");
    }

    return 0;
}
