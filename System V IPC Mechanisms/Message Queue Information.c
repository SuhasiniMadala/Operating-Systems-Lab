/* Program Number: 42
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program prints information about a message queue
   using msqid_ds and ipc_perm structures including permissions,
   UID, GID, timestamps, queue size, etc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds queue_info;  /* Structure to hold queue information */

    printf("===== Message Queue Information =====\n\n");

    /* Create or access the message queue */
    key = ftok(".", 'A');
    if (key == -1) { perror("ftok"); exit(1); }

    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) { perror("msgget"); exit(1); }

    printf("Message Queue ID: %d\n\n", msgid);

    /* msgctl() with IPC_STAT fills the msqid_ds structure with
       all information about the message queue.
       
       struct msqid_ds contains:
       - msg_perm: struct ipc_perm (permissions, UID, GID)
       - msg_stime: time of last msgsnd
       - msg_rtime: time of last msgrcv
       - msg_ctime: time of last change
       - msg_qnum: number of messages in queue
       - msg_qbytes: max bytes allowed in queue
       - msg_lspid: PID of last msgsnd
       - msg_lrpid: PID of last msgrcv */
    if (msgctl(msgid, IPC_STAT, &queue_info) == -1) {
        perror("msgctl IPC_STAT");
        exit(1);
    }

    /* (a) Access permission */
    printf("(a) Access Permission: %o (octal)\n",
           queue_info.msg_perm.mode & 0777);

    /* (b) UID and GID */
    printf("(b) Owner UID: %d\n", queue_info.msg_perm.uid);
    printf("    Owner GID: %d\n", queue_info.msg_perm.gid);
    printf("    Creator UID: %d\n", queue_info.msg_perm.cuid);
    printf("    Creator GID: %d\n", queue_info.msg_perm.cgid);

    /* (c) Time of last message sent and received */
    printf("(c) Last message sent:     %s",
           queue_info.msg_stime ? ctime(&queue_info.msg_stime) : "Never\n");
    printf("    Last message received: %s",
           queue_info.msg_rtime ? ctime(&queue_info.msg_rtime) : "Never\n");

    /* (d) Time of last change */
    printf("(d) Last change time:      %s", ctime(&queue_info.msg_ctime));

    /* (e) Size of the queue (max bytes allowed) */
    printf("(e) Max bytes in queue:    %lu\n",
           (unsigned long)queue_info.msg_qbytes);

    /* (f) Number of messages currently in queue */
    printf("(f) Messages in queue:     %lu\n",
           (unsigned long)queue_info.msg_qnum);

    /* (g) Max bytes allowed - same as (e) */
    printf("(g) Max bytes allowed:     %lu\n",
           (unsigned long)queue_info.msg_qbytes);

    /* (h) PID of last msgsnd and msgrcv */
    printf("(h) PID of last msgsnd:    %d\n", queue_info.msg_lspid);
    printf("    PID of last msgrcv:    %d\n", queue_info.msg_lrpid);

    return 0;
}
