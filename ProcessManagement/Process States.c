/* Program Number: 19
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program demonstrates processes in different states:
   (a) running, (b) sleeping, and (c) stopped. Use ps command to verify.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    printf("===== Process States Demo =====\n");
    printf("Parent PID: %d\n\n", getpid());

    /* (a) RUNNING state (R): A process actively using the CPU.
       We create a child that does continuous computation (busy loop). */
    pid = fork();
    if (pid == 0) {
        /* Child process: busy loop to stay in RUNNING state */
        printf("(a) Running process: PID=%d (check with: ps -o pid,stat,comm -p %d)\n",
               getpid(), getpid());
        volatile long i;
        /* Busy loop - the process keeps computing, staying in 'R' state */
        while (1) {
            for (i = 0; i < 100000000; i++);
        }
        exit(0);
    }

    sleep(1);  /* Give the running child time to start */

    /* (b) SLEEPING state (S): A process waiting for an event (like I/O or timer).
       We create a child that calls sleep(), putting it in interruptible sleep. */
    pid_t pid2 = fork();
    if (pid2 == 0) {
        printf("(b) Sleeping process: PID=%d (check with: ps -o pid,stat,comm -p %d)\n",
               getpid(), getpid());
        /* sleep() puts the process into interruptible sleep state ('S').
           The process is waiting for the timer to expire. */
        sleep(300);  /* Sleep for 5 minutes */
        exit(0);
    }

    sleep(1);

    /* (c) STOPPED state (T): A process that has been paused by a signal.
       We create a child and send it SIGSTOP to stop it. */
    pid_t pid3 = fork();
    if (pid3 == 0) {
        printf("(c) Will-be-stopped process: PID=%d\n", getpid());
        /* This child does nothing special, parent will stop it */
        while (1) sleep(1);
        exit(0);
    }

    sleep(1);

    /* Send SIGSTOP to the third child to put it in stopped state.
       SIGSTOP cannot be caught or ignored - it always stops the process.
       This is what happens when you press Ctrl+Z in the terminal. */
    kill(pid3, SIGSTOP);
    printf("(c) Stopped process: PID=%d (sent SIGSTOP)\n\n", pid3);

    printf("--- Verify process states with these commands: ---\n");
    printf("ps -o pid,stat,comm -p %d,%d,%d\n\n", pid, pid2, pid3);
    printf("States: R=Running, S=Sleeping, T=Stopped\n");
    printf("Press Enter to clean up and exit...\n");
    getchar();

    /* Clean up: kill all child processes */
    kill(pid, SIGKILL);
    kill(pid2, SIGKILL);
    kill(pid3, SIGKILL);   /* Need SIGKILL since it's stopped */
    waitpid(pid, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    printf("All child processes cleaned up.\n");
    return 0;
}
