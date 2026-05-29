/* Program Number: 44
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program receives messages from a message queue.
   (a) With 0 as flag (blocking mode)
   (b) With IPC_NOWAIT as flag (non-blocking mode)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct msgbuf {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key;
    int msgid;
    struct msgbuf msg;

    printf("===== Receiving Messages from Message Queue =====\n\n");

    /* Access the existing message queue (run program 43 first!) */
    key = ftok(".", 'A');
    if (key == -1) { perror("ftok"); exit(1); }

    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget (run program 43 first to create queue and send messages)");
        exit(1);
    }
    printf("Message Queue ID: %d\n\n", msgid);

    /* ======== (a) Receive with flag = 0 (BLOCKING) ======== */
    /* When flag is 0, msgrcv BLOCKS (waits) if no message of the
       requested type is available. It will wait until a message arrives.
       
       msgrcv(msgid, &msg, size, type, flags)
       - type = 0: receive the FIRST message regardless of type
       - type > 0: receive first message with that specific type
       - type < 0: receive message with lowest type <= |type| */
    printf("(a) Receiving with flag = 0 (blocking mode):\n");
    if (msgrcv(msgid, &msg, sizeof(msg.mtext), 0, 0) == -1) {
        perror("msgrcv");
    } else {
        printf("  Received: [type=%ld] %s\n\n", msg.mtype, msg.mtext);
    }

    /* ======== (b) Receive with IPC_NOWAIT (NON-BLOCKING) ======== */
    /* When IPC_NOWAIT is set, msgrcv returns IMMEDIATELY.
       If no message is available, it returns -1 with errno = ENOMSG
       instead of blocking/waiting. */
    printf("(b) Receiving with IPC_NOWAIT flag (non-blocking mode):\n");

    /* Try to receive a message of type 2 */
    if (msgrcv(msgid, &msg, sizeof(msg.mtext), 2, IPC_NOWAIT) == -1) {
        if (errno == ENOMSG) {
            printf("  No message of type 2 available (ENOMSG).\n");
        } else {
            perror("msgrcv");
        }
    } else {
        printf("  Received: [type=%ld] %s\n", msg.mtype, msg.mtext);
    }

    /* Try receiving any remaining message with IPC_NOWAIT */
    printf("\n  Trying to receive any remaining messages (IPC_NOWAIT):\n");
    while (msgrcv(msgid, &msg, sizeof(msg.mtext), 0, IPC_NOWAIT) != -1) {
        printf("  Received: [type=%ld] %s\n", msg.mtype, msg.mtext);
    }
    if (errno == ENOMSG) {
        printf("  No more messages in queue.\n");
    }

    return 0;
}
