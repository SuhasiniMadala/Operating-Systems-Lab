/* Program Number: 46
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program removes (deletes) a message queue
   using msgctl() with IPC_RMID command.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;

    printf("===== Removing a Message Queue =====\n\n");

    /* Access the existing message queue */
    key = ftok(".", 'A');
    if (key == -1) { perror("ftok"); exit(1); }

    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget (no queue to remove)");
        printf("Create one first by running program 41.\n");
        exit(1);
    }
    printf("Found Message Queue ID: %d\n", msgid);

    /* Remove the message queue using IPC_RMID.
       IPC_RMID immediately removes the message queue.
       All messages in the queue are lost.
       Any process waiting in msgsnd/msgrcv will get an error.
       
       This is equivalent to running: ipcrm -q <msgid> */
    printf("Removing message queue...\n");
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID");
        exit(1);
    }

    printf("Message queue %d removed successfully!\n", msgid);
    printf("\nVerify with: ipcs -q (queue should no longer appear)\n");

    return 0;
}
