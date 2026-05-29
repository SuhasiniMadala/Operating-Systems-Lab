/* Program Number: 36
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates FIFO files using (d) mknod system call
   and (e) mkfifo library function. Parts (a), (b), (c) are in 36.sh.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    printf("===== FIFO Creation using System Calls =====\n\n");

    /* (d) Create FIFO using mknod() system call.
       mknod(path, mode, dev) creates a special file.
       S_IFIFO = FIFO file type.
       0666 = read/write for all.
       dev = 0 (device number, ignored for FIFOs). */
    printf("(d) Creating FIFO using mknod() system call...\n");
    if (mknod("fifo_by_mknod", S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
        printf("  (May already exist. Delete it and try again.)\n");
    } else {
        printf("  Created 'fifo_by_mknod' successfully.\n");
    }

    /* (e) Create FIFO using mkfifo() library function.
       mkfifo(path, mode) is a simpler way to create FIFOs.
       Internally, mkfifo typically calls mknod with S_IFIFO. */
    printf("\n(e) Creating FIFO using mkfifo() library function...\n");
    if (mkfifo("fifo_by_mkfifo", 0666) == -1) {
        perror("mkfifo");
        printf("  (May already exist. Delete it and try again.)\n");
    } else {
        printf("  Created 'fifo_by_mkfifo' successfully.\n");
    }

    /* Verify by checking file type with stat */
    struct stat st;
    printf("\n--- Verification ---\n");
    if (stat("fifo_by_mknod", &st) == 0 && S_ISFIFO(st.st_mode))
        printf("fifo_by_mknod: confirmed FIFO\n");
    if (stat("fifo_by_mkfifo", &st) == 0 && S_ISFIFO(st.st_mode))
        printf("fifo_by_mkfifo: confirmed FIFO\n");

    printf("\nUse 'ls -l' to see 'p' prefix indicating FIFO type.\n");
    printf("Clean up: rm fifo_by_mknod fifo_by_mkfifo\n");
    return 0;
}
