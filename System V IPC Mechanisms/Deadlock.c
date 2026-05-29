/* Program Number: 50
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program intentionally creates a deadlock scenario
   using two semaphores. Process 1 locks sem1 then tries sem2.
   Process 2 locks sem2 then tries sem1. Neither can proceed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

/* Union for semctl - must be defined by programmer on Linux */
union semun { int val; struct semid_ds *buf; unsigned short *array; };

/* sem_wait (P operation): decrements semaphore, blocks if 0 */
void sem_wait_op(int semid, int sem_num) {
    struct sembuf sb;
    sb.sem_num = sem_num;
    sb.sem_op = -1;
    sb.sem_flg = 0;  /* Block if not available */
    semop(semid, &sb, 1);
}

/* sem_signal (V operation): increments semaphore, wakes waiters */
void sem_signal_op(int semid, int sem_num) {
    struct sembuf sb;
    sb.sem_num = sem_num;
    sb.sem_op = 1;
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}

int main() {
    key_t key;
    int semid;
    union semun sem_arg;

    printf("===== Deadlock Demonstration with Semaphores =====\n\n");
    printf("WARNING: This program will DEADLOCK intentionally!\n");
    printf("Use Ctrl+C to kill it, then clean up with: ipcs -s / ipcrm\n\n");

    /* Create a semaphore set with 2 semaphores (sem0 and sem1).
       Both initialized to 1 (binary semaphores). */
    key = ftok(".", 'D');
    semid = semget(key, 2, IPC_CREAT | 0666);
    if (semid == -1) { perror("semget"); exit(1); }

    sem_arg.val = 1;
    semctl(semid, 0, SETVAL, sem_arg);  /* Semaphore 0 = 1 */
    semctl(semid, 1, SETVAL, sem_arg);  /* Semaphore 1 = 1 */
    printf("Created 2 binary semaphores (both = 1)\n\n");

    /* Deadlock scenario:
       Process 1: locks Sem0, sleeps, tries to lock Sem1 (blocked!)
       Process 2: locks Sem1, sleeps, tries to lock Sem0 (blocked!)
       
       Neither can proceed because each holds one semaphore
       and waits for the other. This is a circular wait = DEADLOCK. */

    pid_t pid = fork();

    if (pid == 0) {
        /* CHILD (Process 2): Lock Sem1 first, then try Sem0 */
        printf("Process 2 (PID=%d): Locking Semaphore 1...\n", getpid());
        sem_wait_op(semid, 1);
        printf("Process 2: Got Semaphore 1! Sleeping 2 seconds...\n");
        sleep(2);

        printf("Process 2: Now trying to lock Semaphore 0...\n");
        printf("Process 2: *** WILL BLOCK HERE (DEADLOCK!) ***\n");
        sem_wait_op(semid, 0);  /* This will BLOCK forever! */

        /* This line will never be reached due to deadlock */
        printf("Process 2: Got both semaphores (won't happen).\n");
        sem_signal_op(semid, 0);
        sem_signal_op(semid, 1);
        exit(0);
    } else {
        /* PARENT (Process 1): Lock Sem0 first, then try Sem1 */
        printf("Process 1 (PID=%d): Locking Semaphore 0...\n", getpid());
        sem_wait_op(semid, 0);
        printf("Process 1: Got Semaphore 0! Sleeping 2 seconds...\n");
        sleep(2);

        printf("Process 1: Now trying to lock Semaphore 1...\n");
        printf("Process 1: *** WILL BLOCK HERE (DEADLOCK!) ***\n");
        sem_wait_op(semid, 1);  /* This will BLOCK forever! */

        /* This line will never be reached due to deadlock */
        printf("Process 1: Got both semaphores (won't happen).\n");
        sem_signal_op(semid, 1);
        sem_signal_op(semid, 0);
        wait(NULL);

        /* Clean up (won't reach here due to deadlock) */
        semctl(semid, 0, IPC_RMID);
    }

    return 0;
}
