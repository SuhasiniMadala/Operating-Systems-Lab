/* Program Number: 49a
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program rewrites the ticket reservation system
   (from Q17) using semaphores instead of file locking to protect
   the critical section.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

/* Union for semctl - must be defined by programmer on Linux */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

/* sem_wait (P operation): decrements semaphore by 1.
   If value becomes 0, the process blocks until another process
   releases the semaphore. This is the "lock" operation. */
void sem_wait_op(int semid) {
    struct sembuf sb;
    sb.sem_num = 0;     /* Semaphore index (first one) */
    sb.sem_op = -1;     /* Decrement by 1 (wait/lock) */
    sb.sem_flg = 0;     /* Block if value would go below 0 */
    semop(semid, &sb, 1);
}

/* sem_signal (V operation): increments semaphore by 1.
   This wakes up any process waiting on the semaphore.
   This is the "unlock" operation. */
void sem_signal_op(int semid) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;      /* Increment by 1 (signal/unlock) */
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}

void book_ticket(int semid, int process_num) {
    int fd;
    char buffer[20];

    /* Wait on semaphore (enter critical section) */
    printf("Process %d: Waiting for semaphore...\n", process_num);
    sem_wait_op(semid);
    printf("Process %d: Entered critical section.\n", process_num);

    /* Read current ticket number */
    fd = open("ticket_sem.txt", O_RDWR);
    if (fd == -1) { perror("open"); sem_signal_op(semid); return; }

    memset(buffer, 0, sizeof(buffer));
    read(fd, buffer, sizeof(buffer) - 1);
    int ticket = atoi(buffer);
    printf("Process %d: Current ticket = %d\n", process_num, ticket);

    /* Increment and write back */
    ticket++;
    sleep(1);  /* Simulate processing time */
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, 0);
    snprintf(buffer, sizeof(buffer), "%d", ticket);
    write(fd, buffer, strlen(buffer));
    printf("Process %d: New ticket = %d\n", process_num, ticket);
    close(fd);

    /* Signal semaphore (leave critical section) */
    sem_signal_op(semid);
    printf("Process %d: Left critical section.\n\n", process_num);
}

int main() {
    key_t key;
    int semid;
    union semun sem_arg;

    printf("===== Ticket Reservation with Semaphore =====\n\n");

    /* Create and initialize binary semaphore */
    key = ftok(".", 'T');
    semid = semget(key, 1, IPC_CREAT | 0666);
    sem_arg.val = 1;  /* Binary semaphore: 1 = available */
    semctl(semid, 0, SETVAL, sem_arg);

    /* Initialize ticket file */
    int fd = open("ticket_sem.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, "100", 3);
    close(fd);

    /* Create multiple child processes to book tickets concurrently */
    int i;
    for (i = 1; i <= 3; i++) {
        if (fork() == 0) {
            book_ticket(semid, i);
            exit(0);
        }
    }

    /* Wait for all children */
    for (i = 0; i < 3; i++) wait(NULL);

    /* Read final ticket number */
    fd = open("ticket_sem.txt", O_RDONLY);
    char buf[20] = {0};
    read(fd, buf, sizeof(buf) - 1);
    close(fd);
    printf("Final ticket number: %s (should be 103)\n", buf);

    /* Clean up semaphore */
    semctl(semid, 0, IPC_RMID);
    return 0;
}
