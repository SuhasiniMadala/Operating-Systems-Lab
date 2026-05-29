/* Program Number: 47
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates shared memory and performs:
   (a) Write data, (b) Attach read-only and test overwrite,
   (c) Detach, (d) Remove shared memory.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>   /* For shmget, shmat, shmdt, shmctl */

#define SHM_SIZE 1024   /* Size of shared memory segment in bytes */

int main() {
    key_t key;
    int shmid;
    char *shm_ptr;

    printf("===== Shared Memory Operations =====\n\n");

    /* Generate an IPC key */
    key = ftok(".", 'S');
    if (key == -1) { perror("ftok"); exit(1); }

    /* Create a shared memory segment.
       shmget(key, size, flags) creates or accesses shared memory.
       IPC_CREAT: create if it doesn't exist.
       0666: read/write permissions for all. */
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) { perror("shmget"); exit(1); }
    printf("Shared Memory ID: %d\n\n", shmid);

    /* ======== (a) Write data to shared memory ======== */
    printf("(a) Writing data to shared memory...\n");

    /* shmat() attaches the shared memory to our process address space.
       Arguments: shmid, shmaddr (NULL = let kernel choose), flags.
       Returns: pointer to the shared memory region. */
    shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) { perror("shmat"); exit(1); }

    /* Write data directly to shared memory using the pointer */
    strcpy(shm_ptr, "Hello from shared memory! This data is shared between processes.");
    printf("  Written: %s\n\n", shm_ptr);

    /* Detach before re-attaching read-only */
    shmdt(shm_ptr);

    /* ======== (b) Attach read-only and try to overwrite ======== */
    printf("(b) Attaching with SHM_RDONLY and trying to overwrite...\n");

    /* SHM_RDONLY flag makes the attachment read-only.
       Any attempt to write will cause a segmentation fault. */
    shm_ptr = (char *)shmat(shmid, NULL, SHM_RDONLY);
    if (shm_ptr == (char *)-1) { perror("shmat RDONLY"); exit(1); }

    printf("  Read data: %s\n", shm_ptr);
    printf("  Attempting to write to read-only shared memory...\n");
    printf("  (This would cause SIGSEGV/segfault if uncommented)\n");
    /* Uncomment the next line to see the segfault:
       strcpy(shm_ptr, "This will crash!"); */
    printf("  Write attempt skipped to avoid crash.\n\n");

    /* ======== (c) Detach the shared memory ======== */
    printf("(c) Detaching shared memory...\n");
    /* shmdt() detaches the shared memory from our process.
       After this, the pointer is no longer valid.
       The shared memory still exists; it's just not mapped to us. */
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
    } else {
        printf("  Shared memory detached successfully.\n\n");
    }

    /* ======== (d) Remove the shared memory ======== */
    printf("(d) Removing shared memory...\n");
    /* shmctl with IPC_RMID marks the segment for deletion.
       It will be destroyed after all processes detach from it. */
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID");
    } else {
        printf("  Shared memory %d removed successfully.\n", shmid);
    }

    printf("\nAll shared memory operations completed!\n");
    printf("Verify removal with: ipcs -m\n");
    return 0;
}
