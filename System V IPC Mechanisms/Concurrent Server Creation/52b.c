/* Program Number: 52b
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: This program creates a concurrent server using
   pthread_create(). Each client connection is handled by a separate
   thread, allowing multiple clients simultaneously.
   Compile with: gcc -o 52b 52b.c -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>       /* For pthread_create, pthread_detach */

#define PORT 9091
#define BUFFER_SIZE 1024

/* Structure to pass client info to the thread function.
   Each thread needs its own copy of the client fd and address. */
struct client_info {
    int client_fd;
    struct sockaddr_in client_addr;
};

/* Thread function to handle each client.
   This runs in a separate thread, so multiple clients can
   be handled concurrently without forking. */
void *handle_client(void *arg) {
    struct client_info *info = (struct client_info *)arg;
    int client_fd = info->client_fd;
    char buffer[BUFFER_SIZE];
    ssize_t n;

    printf("[Thread %lu] Handling client from %s:%d\n",
           (unsigned long)pthread_self(),
           inet_ntoa(info->client_addr.sin_addr),
           ntohs(info->client_addr.sin_port));

    /* Echo server: read from client, send back with prefix */
    while ((n = read(client_fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        printf("[Thread %lu] Received: %s",
               (unsigned long)pthread_self(), buffer);

        if (strncmp(buffer, "quit", 4) == 0) break;

        /* Send echo reply */
        char reply[BUFFER_SIZE + 50];
        snprintf(reply, sizeof(reply), "[Server Thread %lu] Echo: %s",
                 (unsigned long)pthread_self(), buffer);
        write(client_fd, reply, strlen(reply));
    }

    printf("[Thread %lu] Client disconnected.\n",
           (unsigned long)pthread_self());
    close(client_fd);
    free(info);  /* Free the client info structure */
    return NULL;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t thread_id;

    printf("===== Concurrent Server using pthread_create() =====\n");
    printf("Compile with: gcc -o 52b 52b.c -lpthread\n\n");

    /* Create, bind, and listen */
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

    /* Main loop: accept clients and create a thread for each one.
       Threads are lighter than processes (fork), sharing the same
       memory space. This is more efficient for many clients. */
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == -1) { perror("accept"); continue; }

        printf("[Main] New client connected. Creating thread...\n");

        /* Allocate client info structure for the thread.
           We use malloc because each thread needs its own copy.
           The thread will free this memory when it's done. */
        struct client_info *info = malloc(sizeof(struct client_info));
        info->client_fd = client_fd;
        info->client_addr = client_addr;

        /* Create a new thread to handle this client.
           pthread_create(thread_id, attributes, function, argument)
           The thread runs handle_client() with info as argument. */
        if (pthread_create(&thread_id, NULL, handle_client, info) != 0) {
            perror("pthread_create");
            close(client_fd);
            free(info);
            continue;
        }

        /* Detach the thread so its resources are freed automatically
           when it finishes. We don't need to join (wait for) it. */
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}
