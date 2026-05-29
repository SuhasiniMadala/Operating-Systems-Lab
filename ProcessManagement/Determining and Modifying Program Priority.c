/* Program Number: 28
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program finds out the current priority (nice value)
   of the running process and modifies it using the nice() system call.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>  /* For getpriority, setpriority */
#include <errno.h>

int main() {
    int current_nice, new_nice;

    printf("===== Process Priority (Nice Value) =====\n\n");
    printf("PID: %d\n\n", getpid());

    /* Nice values range from -20 (highest priority) to 19 (lowest priority).
       Default nice value is 0. Only root can set negative nice values.
       Higher nice value = lower priority (you're being "nice" to others). */

    /* Get the current nice value using getpriority().
       PRIO_PROCESS = get priority of a specific process.
       0 = current process. */
    errno = 0;  /* Reset errno because getpriority can return -1 legitimately */
    current_nice = getpriority(PRIO_PROCESS, 0);
    if (current_nice == -1 && errno != 0) {
        perror("getpriority");
        exit(1);
    }
    printf("Current nice value: %d\n", current_nice);

    /* Modify the priority using nice().
       nice(increment) adds 'increment' to the current nice value.
       For example, nice(5) changes nice from 0 to 5.
       Returns the new nice value on success, -1 on error. */
    printf("\nIncreasing nice value by 5 (lowering priority)...\n");
    errno = 0;
    new_nice = nice(5);
    if (new_nice == -1 && errno != 0) {
        perror("nice");
        exit(1);
    }
    printf("New nice value after nice(5): %d\n", new_nice);

    /* Verify using getpriority again */
    current_nice = getpriority(PRIO_PROCESS, 0);
    printf("Verified with getpriority(): %d\n\n", current_nice);

    /* Try increasing nice again */
    printf("Increasing nice value by 10 more...\n");
    errno = 0;
    new_nice = nice(10);
    if (new_nice == -1 && errno != 0) {
        perror("nice");
    } else {
        printf("New nice value after nice(10): %d\n\n", new_nice);
    }

    printf("Note: Nice range is -20 (highest priority) to 19 (lowest).\n");
    printf("      Only root can decrease nice value (increase priority).\n");
    printf("      Use 'nice -n <value> <command>' from the shell.\n");

    return 0;
}
