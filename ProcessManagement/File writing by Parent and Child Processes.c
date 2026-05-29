/* Program Number: 21
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program opens a file, calls fork(), and allows both
   the child and parent processes to write to the same file.
   The output shows interleaved writes from both processes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd;
    pid_t pid;

    /* Open a file BEFORE fork(). This is important because both parent
       and child will SHARE the same file descriptor and file table entry.
       This means they share the same file offset, so their writes will
       be sequential and not overwrite each other. */
    fd = open("fork_write.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    printf("File opened before fork. fd = %d\n\n", fd);

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        /* CHILD process writes to the shared file descriptor */
        int i;
        for (i = 0; i < 5; i++) {
            char msg[50];
            snprintf(msg, sizeof(msg), "Child  (PID=%d) write #%d\n", getpid(), i+1);
            write(fd, msg, strlen(msg));
            usleep(100000);  /* Small delay to show interleaving */
        }
        close(fd);
        exit(0);
    } else {
        /* PARENT process also writes to the same shared file descriptor */
        int i;
        for (i = 0; i < 5; i++) {
            char msg[50];
            snprintf(msg, sizeof(msg), "Parent (PID=%d) write #%d\n", getpid(), i+1);
            write(fd, msg, strlen(msg));
            usleep(100000);
        }

        wait(NULL);  /* Wait for child to finish */
        close(fd);

        /* Read and display the file to see the interleaved output */
        printf("--- Contents of 'fork_write.txt' ---\n");
        int fd2 = open("fork_write.txt", O_RDONLY);
        char buffer[1024];
        ssize_t n;
        while ((n = read(fd2, buffer, sizeof(buffer)-1)) > 0) {
            buffer[n] = '\0';
            printf("%s", buffer);
        }
        close(fd2);
        printf("--- End ---\n");
        printf("\nNotice: Writes from parent and child may be interleaved\n");
        printf("because both share the same file offset.\n");
    }

    return 0;
}
