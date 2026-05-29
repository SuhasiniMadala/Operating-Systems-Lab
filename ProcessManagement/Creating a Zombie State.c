/* Program Number: 22
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates a zombie process. A zombie is a child
   process that has terminated but its parent hasn't called wait() yet.
   The child's entry remains in the process table until the parent reads it.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    printf("===== Creating a Zombie Process =====\n\n");

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        /* CHILD process: exits immediately.
           When a child exits but the parent hasn't called wait(),
           the child becomes a ZOMBIE (also called "defunct" process).
           The child's process table entry is kept so the parent can
           later retrieve its exit status. The zombie state is shown
           as 'Z' or 'Z+' in the ps command. */
        printf("Child (PID=%d): I am exiting now to become a zombie!\n", getpid());
        exit(0);  /* Child exits immediately */
    } else {
        /* PARENT process: does NOT call wait().
           This means the child's exit status is never collected,
           and the child stays in zombie state. */
        printf("Parent (PID=%d): My child (PID=%d) has exited.\n", getpid(), pid);
        printf("Parent: I am NOT calling wait(), so child becomes a zombie.\n\n");
        printf("--- Check zombie with: ps -eo pid,ppid,stat,comm | grep -i defunct ---\n");
        printf("--- Or run: ps -o pid,stat,comm -p %d ---\n\n", pid);
        printf("Parent sleeping for 30 seconds so you can check...\n");
        printf("The zombie will show state 'Z' in ps output.\n");

        /* Sleep so the user can observe the zombie in the process table */
        sleep(30);

        /* After 30 seconds, the parent exits too.
           When the parent exits, the zombie is adopted by init (PID 1),
           which immediately calls wait() and removes it. */
        printf("Parent exiting now. Zombie will be cleaned up by init.\n");
    }

    return 0;
}
