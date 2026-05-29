/* Program Number: 30
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates a daemon process that executes
   a task (runs a script/command) at a specific time.
   A daemon runs in the background without a controlling terminal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <signal.h>

int main() {
    pid_t pid, sid;
    int target_hour, target_min;
    time_t now;
    struct tm *tm_info;

    /* Get the target time from user before becoming a daemon
       (we won't have a terminal after daemonizing) */
    printf("===== Daemon Process: Run Script at Specific Time =====\n\n");
    printf("Enter target time (24-hour format)\n");
    printf("Hour (0-23): ");
    scanf("%d", &target_hour);
    printf("Minute (0-59): ");
    scanf("%d", &target_min);

    time(&now);
    tm_info = localtime(&now);
    printf("\nCurrent time: %02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    printf("Task will run at: %02d:%02d\n", target_hour, target_min);
    printf("Daemon PID will be printed below. Check /tmp/daemon.log for output.\n\n");

    /* Step 1: Fork the parent process.
       The child will become the daemon. The parent exits immediately,
       making the child an orphan (adopted by init). */
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid > 0) {
        /* Parent exits. Child continues as daemon. */
        printf("Daemon started with PID: %d\n", pid);
        printf("Parent exiting. Daemon running in background.\n");
        exit(0);
    }

    /* Step 2: Create a new session.
       setsid() makes the child a session leader and detaches it
       from the controlling terminal. This is essential for a daemon. */
    sid = setsid();
    if (sid < 0) {
        exit(1);
    }

    /* Step 3: Change working directory to root.
       This prevents the daemon from holding a mount point busy. */
    chdir("/");

    /* Step 4: Set file creation mask to 0.
       umask(0) ensures the daemon can create files with any permissions. */
    umask(0);

    /* Step 5: Close standard file descriptors.
       A daemon has no terminal, so stdin/stdout/stderr are useless.
       Redirect them to /dev/null. */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDONLY);  /* stdin  -> /dev/null */
    open("/tmp/daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0644); /* stdout -> log */
    open("/tmp/daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0644); /* stderr -> log */

    /* Step 6: Daemon main loop.
       Check the time every 30 seconds. When the target time is reached,
       execute the task. */
    printf("Daemon started. PID=%d. Waiting for %02d:%02d\n", getpid(), target_hour, target_min);
    fflush(stdout);

    while (1) {
        time(&now);
        tm_info = localtime(&now);

        /* Check if current time matches target time */
        if (tm_info->tm_hour == target_hour && tm_info->tm_min == target_min) {
            printf("Time reached! Executing task at %02d:%02d:%02d\n",
                   tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
            fflush(stdout);

            /* Execute the task - here we run a simple command.
               You can replace this with system("./your_script.sh") */
            system("echo 'Daemon task executed!' >> /tmp/daemon_task_output.txt");
            system("date >> /tmp/daemon_task_output.txt");

            printf("Task completed. Check /tmp/daemon_task_output.txt\n");
            fflush(stdout);

            /* Exit after executing the task (one-shot daemon) */
            break;
        }

        sleep(30);  /* Check every 30 seconds */
    }

    printf("Daemon exiting.\n");
    return 0;
}
