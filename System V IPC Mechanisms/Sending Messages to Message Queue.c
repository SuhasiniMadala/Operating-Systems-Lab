/* Program Number: 43
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program sends messages to a message queue using
   msgsnd(). Verify using $ipcs -q after running.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/* Message structure. The first field MUST be a long (message type).
   The message type is used by msgrcv to selectively receive messages. */
struct msgbuf {
    long mtype;       /* Message type (must be > 0) */
    char mtext[100];  /* Message data */
};

int main() {
    key_t key;
    int msgid;
    struct msgbuf msg;

    printf("===== Sending Messages to Message Queue =====\n\n");

    /* Create or access the message queue */
    key = ftok(".", 'A');
    if (key == -1) { perror("ftok"); exit(1); }

    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) { perror("msgget"); exit(1); }
    printf("Message Queue ID: %d\n\n", msgid);

    /* Send three messages with different types */

    /* Message 1: type 1 */
    msg.mtype = 1;  /* Message type 1 (like a priority/category) */
    strcpy(msg.mtext, "Hello! This is message type 1.");
    /* msgsnd(msgid, &msg, size_of_data, flags)
       - msgid: queue ID
       - &msg: pointer to message structure
       - sizeof(msg.mtext): size of data portion (NOT including mtype)
       - 0: flags (0 = block if queue is full) */
    if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }
    printf("Sent: [type=%ld] %s\n", msg.mtype, msg.mtext);

    /* Message 2: type 2 */
    msg.mtype = 2;
    strcpy(msg.mtext, "This is an urgent message (type 2).");
    if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
        perror("msgsnd"); exit(1);
    }
    printf("Sent: [type=%ld] %s\n", msg.mtype, msg.mtext);

    /* Message 3: type 1 again */
    msg.mtype = 1;
    strcpy(msg.mtext, "Another message of type 1.");
    if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
        perror("msgsnd"); exit(1);
    }
    printf("Sent: [type=%ld] %s\n", msg.mtype, msg.mtext);

    printf("\nAll messages sent! Check with: ipcs -q\n");
    printf("The 'messages' column should show 3.\n");

    return 0;
}
