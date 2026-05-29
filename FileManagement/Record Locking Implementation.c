/* Program Number: 18
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program performs record locking. It creates three
   records in a file, then demonstrates write lock and read lock on
   individual records to avoid race conditions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

/* Each record is a fixed-size structure */
struct Record {
    int id;
    char name[20];
    float salary;
};

/* Function to lock a specific record in the file.
   record_num: which record (0-based index)
   lock_type: F_RDLCK (read) or F_WRLCK (write) */
int lock_record(int fd, int record_num, short lock_type) {
    struct flock lock;
    lock.l_type = lock_type;
    lock.l_whence = SEEK_SET;
    /* Calculate the byte offset of the record */
    lock.l_start = record_num * sizeof(struct Record);
    lock.l_len = sizeof(struct Record);  /* Lock only this record's bytes */

    /* F_SETLKW = wait if lock is held by another process */
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl lock");
        return -1;
    }
    return 0;
}

/* Function to unlock a specific record */
int unlock_record(int fd, int record_num) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = record_num * sizeof(struct Record);
    lock.l_len = sizeof(struct Record);

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl unlock");
        return -1;
    }
    return 0;
}

int main() {
    int fd;
    struct Record records[3];

    /* Create three records */
    records[0].id = 1; strcpy(records[0].name, "Alice");   records[0].salary = 50000.0;
    records[1].id = 2; strcpy(records[1].name, "Bob");     records[1].salary = 60000.0;
    records[2].id = 3; strcpy(records[2].name, "Charlie"); records[2].salary = 70000.0;

    /* Create the file and write the three records */
    fd = open("records.dat", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { perror("open"); exit(1); }

    write(fd, records, sizeof(records));
    printf("Created 3 records in 'records.dat'\n\n");

    /* ======== (a) WRITE LOCK on record 1 ======== */
    printf("(a) Acquiring WRITE LOCK on Record 2 (Bob)...\n");
    if (lock_record(fd, 1, F_WRLCK) == 0) {
        printf("  Write lock acquired on Record 2.\n");

        /* Read the record */
        struct Record rec;
        lseek(fd, 1 * sizeof(struct Record), SEEK_SET);
        read(fd, &rec, sizeof(struct Record));
        printf("  Before: ID=%d, Name=%s, Salary=%.2f\n", rec.id, rec.name, rec.salary);

        /* Modify the record while holding the lock */
        rec.salary = 65000.0;
        lseek(fd, 1 * sizeof(struct Record), SEEK_SET);
        write(fd, &rec, sizeof(struct Record));
        printf("  After:  ID=%d, Name=%s, Salary=%.2f\n", rec.id, rec.name, rec.salary);

        /* Unlock the record */
        unlock_record(fd, 1);
        printf("  Write lock released on Record 2.\n\n");
    }

    /* ======== (b) READ LOCK on record 2 ======== */
    printf("(b) Acquiring READ LOCK on Record 3 (Charlie)...\n");
    if (lock_record(fd, 2, F_RDLCK) == 0) {
        printf("  Read lock acquired on Record 3.\n");

        /* Read the record (read-only access) */
        struct Record rec;
        lseek(fd, 2 * sizeof(struct Record), SEEK_SET);
        read(fd, &rec, sizeof(struct Record));
        printf("  Reading: ID=%d, Name=%s, Salary=%.2f\n", rec.id, rec.name, rec.salary);
        printf("  (Other processes can also read this record simultaneously)\n");

        /* Unlock the record */
        unlock_record(fd, 2);
        printf("  Read lock released on Record 3.\n\n");
    }

    /* Display all records */
    printf("--- All Records ---\n");
    lseek(fd, 0, SEEK_SET);
    int i;
    for (i = 0; i < 3; i++) {
        struct Record rec;
        read(fd, &rec, sizeof(struct Record));
        printf("Record %d: ID=%d, Name=%-10s, Salary=%.2f\n",
               i+1, rec.id, rec.name, rec.salary);
    }

    close(fd);
    return 0;
}
