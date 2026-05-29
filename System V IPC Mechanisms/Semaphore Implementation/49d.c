/* Program Number: 49d
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program removes a previously created semaphore
   using semctl() with IPC_RMID command.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
    key_t key;
    int semid;

    printf("===== Removing Semaphore =====\n\n");

    /* Access the existing semaphore (use same key as creation) */
    key = ftok(".", 'B');
    if (key == -1) { perror("ftok"); exit(1); }

    semid = semget(key, 1, 0666);
    if (semid == -1) {
        perror("semget (no semaphore found - create one first with Q48)");
        exit(1);
    }

    printf("Found Semaphore ID: %d\n", semid);
    printf("Current value: %d\n", semctl(semid, 0, GETVAL));

    /* Remove the semaphore using IPC_RMID.
       All processes waiting on this semaphore will get an error.
       Equivalent to: ipcrm -s <semid> */
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID");
        exit(1);
    }

    printf("Semaphore %d removed successfully!\n", semid);
    printf("\nVerify with: ipcs -s\n");
    return 0;
}
