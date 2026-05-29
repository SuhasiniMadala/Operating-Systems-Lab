/* Program Number: 51 (Server)
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: Server program for inter-machine communication using
   sockets. The server listens for connections and exchanges messages
   with the client. Run this first, then run the client.
   Usage: ./server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>    /* For socket, bind, listen, accept */
#include <netinet/in.h>    /* For sockaddr_in, htons */
#include <arpa/inet.h>     /* For inet_ntoa */

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    printf("===== Socket Server =====\n\n");

    /* Step 1: Create a socket.
       socket(domain, type, protocol)
       - AF_INET: IPv4 Internet protocol
       - SOCK_STREAM: TCP (reliable, connection-oriented)
       - 0: default protocol for the given type (TCP for SOCK_STREAM) */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }
    printf("Socket created. fd = %d\n", server_fd);

    /* Allow port reuse (avoids "Address already in use" error) */
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* Step 2: Bind the socket to an address and port.
       This tells the OS which port to listen on.
       INADDR_ANY means listen on ALL network interfaces. */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;         /* IPv4 */
    server_addr.sin_addr.s_addr = INADDR_ANY; /* Any interface */
    server_addr.sin_port = htons(PORT);       /* Port (host to network byte order) */

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(1);
    }
    printf("Bound to port %d\n", PORT);

    /* Step 3: Listen for incoming connections.
       The second argument (5) is the backlog - max queued connections. */
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        exit(1);
    }
    printf("Listening for connections...\n\n");

    /* Step 4: Accept a client connection.
       accept() blocks until a client connects. Returns a NEW socket
       fd specifically for this client connection. */
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        exit(1);
    }
    printf("Client connected from %s:%d\n\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    /* Step 5: Exchange messages with the client */
    while (1) {
        /* Receive message from client */
        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        printf("Client: %s", buffer);

        if (strncmp(buffer, "quit", 4) == 0) break;

        /* Send reply to client */
        printf("Server> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(client_fd, buffer, strlen(buffer));

        if (strncmp(buffer, "quit", 4) == 0) break;
    }

    /* Close connections */
    close(client_fd);
    close(server_fd);
    printf("Server closed.\n");
    return 0;
}
