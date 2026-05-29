/* Program Number: 52a
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates a concurrent server using fork().
   Each client connection is handled by a separate child process,
   allowing multiple clients to connect simultaneously.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 9090
#define BUFFER_SIZE 1024

/* Signal handler to clean up zombie child processes.
   When a child process (handling a client) exits, it becomes a zombie.
   This handler calls waitpid to clean them up automatically. */
void sigchld_handler(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

/* Function to handle each client connection.
   This runs in a child process, so each client gets its own process. */
void handle_client(int client_fd, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    ssize_t n;

    printf("[Child PID=%d] Handling client from %s:%d\n",
           getpid(), inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    /* Echo server: read from client, send back with prefix */
    while ((n = read(client_fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        printf("[Child PID=%d] Received: %s", getpid(), buffer);

        if (strncmp(buffer, "quit", 4) == 0) break;

        /* Send echo reply */
        char reply[BUFFER_SIZE + 50];
        snprintf(reply, sizeof(reply), "[Server PID=%d] Echo: %s", getpid(), buffer);
        write(client_fd, reply, strlen(reply));
    }

    printf("[Child PID=%d] Client disconnected.\n", getpid());
    close(client_fd);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pid_t pid;

    printf("===== Concurrent Server using fork() =====\n\n");

    /* Set up SIGCHLD handler to prevent zombie processes */
    signal(SIGCHLD, sigchld_handler);

    /* Create, bind, and listen (same as simple server) */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind"); exit(1);
    }
    if (listen(server_fd, 10) == -1) {
        perror("listen"); exit(1);
    }
    printf("Server listening on port %d...\n", PORT);
    printf("Connect with: nc localhost %d\n\n", PORT);

    /* Main loop: accept clients and fork a child for each one.
       The parent continues to accept new connections while
       children handle existing ones concurrently. */
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == -1) { perror("accept"); continue; }

        printf("[Parent] New client connected. Forking child...\n");

        pid = fork();
        if (pid == 0) {
            /* CHILD process: handle this client */
            close(server_fd);  /* Child doesn't need the listening socket */
            handle_client(client_fd, client_addr);
            exit(0);
        } else if (pid > 0) {
            /* PARENT process: close client fd (child has its own copy)
               and go back to accepting new connections */
            close(client_fd);
        } else {
            perror("fork");
            close(client_fd);
        }
    }

    close(server_fd);
    return 0;
}
