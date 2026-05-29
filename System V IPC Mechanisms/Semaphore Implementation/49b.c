/* Program Number: 49b
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program protects shared memory from concurrent
   write access using a binary semaphore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

/* Union for semctl - must be defined by programmer on Linux */
union semun { int val; struct semid_ds *buf; unsigned short *array; };

void sem_wait_op(int semid) {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void sem_signal_op(int semid) {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

int main() {
    key_t key_shm, key_sem;
    int shmid, semid;
    union semun sem_arg;
    char *shm_ptr;

    printf("===== Protecting Shared Memory with Semaphore =====\n\n");

    /* Create shared memory */
    key_shm = ftok(".", 'M');
    shmid = shmget(key_shm, 256, IPC_CREAT | 0666);
    if (shmid == -1) { perror("shmget"); exit(1); }

    /* Create binary semaphore for mutual exclusion */
    key_sem = ftok(".", 'X');
    semid = semget(key_sem, 1, IPC_CREAT | 0666);
    sem_arg.val = 1;
    semctl(semid, 0, SETVAL, sem_arg);

    /* Initialize shared memory */
    shm_ptr = (char *)shmat(shmid, NULL, 0);
    strcpy(shm_ptr, "Initial data");
    shmdt(shm_ptr);

    /* Fork child processes that write to shared memory */
    int i;
    for (i = 1; i <= 3; i++) {
        if (fork() == 0) {
            char *ptr = (char *)shmat(shmid, NULL, 0);

            /* Lock semaphore before writing (critical section) */
            printf("Process %d (PID=%d): Waiting for semaphore...\n", i, getpid());
            sem_wait_op(semid);

            printf("Process %d: Writing to shared memory...\n", i);
            char msg[100];
            snprintf(msg, sizeof(msg), "Written by Process %d (PID=%d)", i, getpid());
            strcpy(ptr, msg);
            printf("Process %d: Wrote: %s\n", i, ptr);
            sleep(1);  /* Simulate work */

            /* Unlock semaphore */
            sem_signal_op(semid);
            printf("Process %d: Released semaphore.\n\n", i);

            shmdt(ptr);
            exit(0);
        }
    }

    /* Wait for all children */
    for (i = 0; i < 3; i++) wait(NULL);

    /* Read final value */
    shm_ptr = (char *)shmat(shmid, NULL, SHM_RDONLY);
    printf("Final shared memory content: %s\n", shm_ptr);
    shmdt(shm_ptr);

    /* Clean up */
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    printf("Shared memory and semaphore removed.\n");
    return 0;
}
