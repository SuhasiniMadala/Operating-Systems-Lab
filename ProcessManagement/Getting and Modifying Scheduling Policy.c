/* Program Number: 29
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program gets the current scheduling policy of the
   process and modifies it to SCHED_FIFO and SCHED_RR.
   Note: Changing to real-time policies requires root privileges.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>     /* For sched_getscheduler, sched_setscheduler */
#include <unistd.h>
#include <string.h>

/* Helper function to convert policy number to readable name */
const char* policy_name(int policy) {
    switch (policy) {
        case SCHED_OTHER: return "SCHED_OTHER (default, CFS)";
        case SCHED_FIFO:  return "SCHED_FIFO (real-time FIFO)";
        case SCHED_RR:    return "SCHED_RR (real-time Round Robin)";
        default:          return "Unknown";
    }
}

int main() {
    int policy;
    struct sched_param param;

    printf("===== Scheduling Policy Demo =====\n");
    printf("PID: %d\n\n", getpid());

    /* Get current scheduling policy.
       sched_getscheduler(pid) returns the policy for the given PID.
       PID of 0 means the calling process. */
    policy = sched_getscheduler(0);
    if (policy == -1) {
        perror("sched_getscheduler");
        exit(1);
    }
    printf("Current scheduling policy: %s\n\n", policy_name(policy));

    /* Get current scheduling parameters (priority) */
    sched_getparam(0, &param);
    printf("Current scheduling priority: %d\n\n", param.sched_priority);

    /* Try to change to SCHED_FIFO.
       sched_setscheduler(pid, policy, &param)
       - pid: 0 = current process
       - policy: the new policy
       - param: scheduling parameters (priority)
       
       Real-time policies require root/sudo privileges. */
    printf("--- Attempting to change to SCHED_FIFO ---\n");
    param.sched_priority = 10;  /* Set priority (1-99 for real-time) */

    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler SCHED_FIFO (needs root)");
    } else {
        policy = sched_getscheduler(0);
        printf("New policy: %s, Priority: %d\n", policy_name(policy), param.sched_priority);
    }

    /* Try to change to SCHED_RR */
    printf("\n--- Attempting to change to SCHED_RR ---\n");
    param.sched_priority = 20;

    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        perror("sched_setscheduler SCHED_RR (needs root)");
    } else {
        policy = sched_getscheduler(0);
        printf("New policy: %s, Priority: %d\n", policy_name(policy), param.sched_priority);
    }

    /* Change back to SCHED_OTHER */
    printf("\n--- Changing back to SCHED_OTHER ---\n");
    param.sched_priority = 0;  /* SCHED_OTHER only allows priority 0 */

    if (sched_setscheduler(0, SCHED_OTHER, &param) == -1) {
        perror("sched_setscheduler SCHED_OTHER");
    } else {
        policy = sched_getscheduler(0);
        printf("Restored policy: %s\n", policy_name(policy));
    }

    printf("\nNote: Run with 'sudo' to change to real-time policies.\n");
    return 0;
}
