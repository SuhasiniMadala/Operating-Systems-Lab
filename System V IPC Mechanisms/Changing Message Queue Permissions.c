/* Program Number: 45
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program changes the existing message queue
   permissions using msgctl() and the msqid_ds structure.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds queue_info;

    printf("===== Changing Message Queue Permissions =====\n\n");

    /* Create or access the message queue */
    key = ftok(".", 'A');
    if (key == -1) { perror("ftok"); exit(1); }

    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) { perror("msgget"); exit(1); }
    printf("Message Queue ID: %d\n\n", msgid);

    /* Get current queue info */
    if (msgctl(msgid, IPC_STAT, &queue_info) == -1) {
        perror("msgctl IPC_STAT");
        exit(1);
    }
    printf("Current permissions: %o (octal)\n", queue_info.msg_perm.mode & 0777);

    /* Change the permissions to 0644 (owner rw, group r, others r).
       We modify the msg_perm.mode field in the msqid_ds structure
       and then apply it using msgctl with IPC_SET. */
    printf("Changing permissions to 0644...\n");
    queue_info.msg_perm.mode = 0644;

    /* IPC_SET applies the changes from our modified msqid_ds structure.
       Only the owner or root can change permissions. */
    if (msgctl(msgid, IPC_SET, &queue_info) == -1) {
        perror("msgctl IPC_SET");
        exit(1);
    }
    printf("Permissions changed successfully!\n\n");

    /* Verify the change by reading info again */
    if (msgctl(msgid, IPC_STAT, &queue_info) == -1) {
        perror("msgctl IPC_STAT");
        exit(1);
    }
    printf("New permissions: %o (octal)\n", queue_info.msg_perm.mode & 0777);
    printf("\nVerify with: ipcs -q\n");

    return 0;
}
