/* Program Number: 27
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program retrieves and displays the maximum and
   minimum real-time priority values for SCHED_FIFO and SCHED_RR
   scheduling policies.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>    /* For sched_get_priority_max/min */

int main() {
    int max_prio, min_prio;

    printf("===== Real-Time Priority Ranges =====\n\n");

    /* sched_get_priority_max() returns the maximum priority value
       for a given scheduling policy.
       sched_get_priority_min() returns the minimum priority value.

       Real-time scheduling policies:
       - SCHED_FIFO: First-In-First-Out real-time scheduling.
         Higher priority processes run first. Same priority = FIFO order.
       - SCHED_RR: Round-Robin real-time scheduling.
         Like FIFO but with time slices (time quantum) for same priority.
       - SCHED_OTHER: Default non-real-time scheduling (CFS on Linux). */

    /* SCHED_FIFO priority range */
    max_prio = sched_get_priority_max(SCHED_FIFO);
    min_prio = sched_get_priority_min(SCHED_FIFO);
    printf("SCHED_FIFO:\n");
    printf("  Minimum priority: %d\n", min_prio);
    printf("  Maximum priority: %d\n\n", max_prio);

    /* SCHED_RR priority range */
    max_prio = sched_get_priority_max(SCHED_RR);
    min_prio = sched_get_priority_min(SCHED_RR);
    printf("SCHED_RR:\n");
    printf("  Minimum priority: %d\n", min_prio);
    printf("  Maximum priority: %d\n\n", max_prio);

    /* SCHED_OTHER (default) priority range */
    max_prio = sched_get_priority_max(SCHED_OTHER);
    min_prio = sched_get_priority_min(SCHED_OTHER);
    printf("SCHED_OTHER (default):\n");
    printf("  Minimum priority: %d\n", min_prio);
    printf("  Maximum priority: %d\n\n", max_prio);

    printf("Note: On Linux, SCHED_FIFO and SCHED_RR range is typically 1-99.\n");
    printf("      SCHED_OTHER is always 0 (uses nice values instead).\n");

    return 0;
}
