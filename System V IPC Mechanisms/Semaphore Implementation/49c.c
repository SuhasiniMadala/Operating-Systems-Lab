/* Program Number: 49c
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program protects multiple pseudo resources (2 resources)
   using a counting semaphore. Only 2 processes can access resources
   simultaneously; others must wait.
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

void sem_wait_op(int semid) {
    struct sembuf sb = {0, -1, 0};  /* Decrement (acquire one resource) */
    semop(semid, &sb, 1);
}

void sem_signal_op(int semid) {
    struct sembuf sb = {0, 1, 0};   /* Increment (release one resource) */
    semop(semid, &sb, 1);
}

void access_resource(int semid, int proc_num) {
    printf("Process %d: Trying to acquire resource...\n", proc_num);
    sem_wait_op(semid);
    printf("Process %d: *** ACQUIRED resource! Using it for 3 seconds ***\n", proc_num);
    sleep(3);  /* Simulate using the resource */
    printf("Process %d: Done. Releasing resource.\n", proc_num);
    sem_signal_op(semid);
}

int main() {
    key_t key;
    int semid;
    union semun sem_arg;
    int i;

    printf("===== Counting Semaphore: 2 Resources, 5 Processes =====\n\n");

    /* Create counting semaphore initialized to 2.
       This means only 2 processes can hold the resource at any time.
       The 3rd process must wait until one of the first two releases. */
    key = ftok(".", 'R');
    semid = semget(key, 1, IPC_CREAT | 0666);
    sem_arg.val = 2;  /* 2 resources available */
    semctl(semid, 0, SETVAL, sem_arg);
    printf("Counting semaphore created with value 2 (2 resources).\n\n");

    /* Create 5 child processes competing for 2 resources */
    for (i = 1; i <= 5; i++) {
        if (fork() == 0) {
            access_resource(semid, i);
            exit(0);
        }
        usleep(100000);  /* Small delay for ordered output */
    }

    /* Wait for all children */
    for (i = 0; i < 5; i++) wait(NULL);

    printf("\nAll processes done. Only 2 could run simultaneously!\n");

    /* Clean up */
    semctl(semid, 0, IPC_RMID);
    printf("Semaphore removed.\n");
    return 0;
}
