/* Program Number: 24
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates three child processes. The parent
   waits for a specific child using waitpid() system call.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;
    int status;

    printf("===== Creating 3 Children and Using waitpid() =====\n");
    printf("Parent PID: %d\n\n", getpid());

    /* Create first child */
    pid1 = fork();
    if (pid1 == 0) {
        printf("Child 1 (PID=%d): Sleeping for 3 seconds...\n", getpid());
        sleep(3);
        printf("Child 1 (PID=%d): Exiting with status 10.\n", getpid());
        exit(10);
    }

    /* Create second child */
    pid2 = fork();
    if (pid2 == 0) {
        printf("Child 2 (PID=%d): Sleeping for 1 second...\n", getpid());
        sleep(1);
        printf("Child 2 (PID=%d): Exiting with status 20.\n", getpid());
        exit(20);
    }

    /* Create third child */
    pid3 = fork();
    if (pid3 == 0) {
        printf("Child 3 (PID=%d): Sleeping for 5 seconds...\n", getpid());
        sleep(5);
        printf("Child 3 (PID=%d): Exiting with status 30.\n", getpid());
        exit(30);
    }

    /* Parent process: all three children are now running */
    printf("Parent: Created children: PID1=%d, PID2=%d, PID3=%d\n\n", pid1, pid2, pid3);

    /* waitpid() waits for a SPECIFIC child process.
       Unlike wait() which waits for ANY child, waitpid() lets us
       choose WHICH child to wait for.
       
       Syntax: waitpid(pid, &status, options)
       - pid > 0: wait for the child with this specific PID
       - pid = -1: wait for any child (same as wait())
       - pid = 0: wait for any child in the same process group
       - status: stores the exit status of the child
       - options: 0 = block until child exits, WNOHANG = don't block */

    /* Wait specifically for Child 2 (should finish first since it sleeps 1s) */
    printf("Parent: Waiting specifically for Child 2 (PID=%d)...\n", pid2);
    waitpid(pid2, &status, 0);
    if (WIFEXITED(status)) {
        printf("Parent: Child 2 exited with status %d\n\n", WEXITSTATUS(status));
    }

    /* Now wait for Child 1 */
    printf("Parent: Waiting specifically for Child 1 (PID=%d)...\n", pid1);
    waitpid(pid1, &status, 0);
    if (WIFEXITED(status)) {
        printf("Parent: Child 1 exited with status %d\n\n", WEXITSTATUS(status));
    }

    /* Finally wait for Child 3 */
    printf("Parent: Waiting specifically for Child 3 (PID=%d)...\n", pid3);
    waitpid(pid3, &status, 0);
    if (WIFEXITED(status)) {
        printf("Parent: Child 3 exited with status %d\n\n", WEXITSTATUS(status));
    }

    printf("Parent: All children have exited. Done!\n");
    return 0;
}
