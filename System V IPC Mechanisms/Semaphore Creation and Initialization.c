/* Program Number: 48
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates and initializes semaphores.
   (a) Create a binary semaphore (value 0 or 1)
   (b) Create a counting semaphore (value can be > 1)
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>   /* For semget, semctl */

/* Union for semctl operations.
   This must be defined by the programmer on Linux
   (it is not provided by the system headers). */
union semun {
    int val;                /* Value for SETVAL */
    struct semid_ds *buf;   /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array;  /* Array for GETALL, SETALL */
};

int main() {
    key_t key;
    int semid_binary, semid_counting;
    union semun sem_arg;

    printf("===== Semaphore Creation and Initialization =====\n\n");

    /* ======== (a) Binary Semaphore ======== */
    /* A binary semaphore can only be 0 or 1.
       Value 1 = resource available (unlocked)
       Value 0 = resource taken (locked)
       Used for mutual exclusion (like a mutex). */
    printf("(a) Creating BINARY semaphore...\n");

    key = ftok(".", 'B');
    if (key == -1) { perror("ftok"); exit(1); }

    /* semget(key, nsems, flags)
       - key: IPC key
       - nsems: number of semaphores in the set (1 for binary)
       - flags: IPC_CREAT | permissions */
    semid_binary = semget(key, 1, IPC_CREAT | 0666);
    if (semid_binary == -1) { perror("semget binary"); exit(1); }
    printf("  Binary Semaphore ID: %d\n", semid_binary);

    /* Initialize the binary semaphore to 1 (available).
       semctl(semid, sem_num, SETVAL, value)
       - sem_num: which semaphore in the set (0 = first)
       - SETVAL: set the value
       - val: the initial value (1 for binary) */
    sem_arg.val = 1;
    if (semctl(semid_binary, 0, SETVAL, sem_arg) == -1) {
        perror("semctl SETVAL");
        exit(1);
    }
    printf("  Initialized to: %d (1 = available)\n\n",
           semctl(semid_binary, 0, GETVAL));

    /* ======== (b) Counting Semaphore ======== */
    /* A counting semaphore can have values > 1.
       Used to control access to multiple identical resources.
       Value N = N resources available.
       Example: 5 printers, semaphore value = 5. */
    printf("(b) Creating COUNTING semaphore (3 resources)...\n");

    key = ftok(".", 'C');
    if (key == -1) { perror("ftok"); exit(1); }

    /* Create a semaphore set with 1 semaphore */
    semid_counting = semget(key, 1, IPC_CREAT | 0666);
    if (semid_counting == -1) { perror("semget counting"); exit(1); }
    printf("  Counting Semaphore ID: %d\n", semid_counting);

    /* Initialize to 3 (3 resources available) */
    sem_arg.val = 3;
    if (semctl(semid_counting, 0, SETVAL, sem_arg) == -1) {
        perror("semctl SETVAL");
        exit(1);
    }
    printf("  Initialized to: %d (3 resources available)\n",
           semctl(semid_counting, 0, GETVAL));

    printf("\nBoth semaphores created! Verify with: ipcs -s\n");
    printf("Remove with: ipcrm -s <semid>\n");
    return 0;
}
