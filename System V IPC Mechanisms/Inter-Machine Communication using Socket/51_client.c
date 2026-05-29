/* Program Number: 51 (Client)
   Student Name: Suhasini Madala; Register Number: BT2024043
   Date: 2-5-26
   Description: Client program for inter-machine communication using
   sockets. The client connects to the server and exchanges messages.
   Run the server first, then run this client.
   Usage: ./client [server_ip]
   Default server IP: 127.0.0.1 (localhost)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    const char *server_ip = "127.0.0.1";  /* Default: localhost */

    /* Allow user to specify server IP as command line argument */
    if (argc > 1) {
        server_ip = argv[1];
    }

    printf("===== Socket Client =====\n\n");

    /* Step 1: Create a socket (same as server) */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(1);
    }
    printf("Socket created. fd = %d\n", sock_fd);

    /* Step 2: Set up server address to connect to */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    /* Convert IP address string to binary form */
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        printf("Invalid server IP: %s\n", server_ip);
        exit(1);
    }

    /* Step 3: Connect to the server.
       connect() initiates a TCP connection to the server.
       It blocks until the connection is established or fails. */
    printf("Connecting to %s:%d...\n", server_ip, PORT);
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect (is the server running?)");
        close(sock_fd);
        exit(1);
    }
    printf("Connected to server!\n\n");
    printf("Type messages to send (type 'quit' to exit):\n\n");

    /* Step 4: Exchange messages with the server */
    while (1) {
        /* Send message to server */
        printf("Client> ");
        fgets(buffer, BUFFER_SIZE, stdin);
        write(sock_fd, buffer, strlen(buffer));

        if (strncmp(buffer, "quit", 4) == 0) break;

        /* Receive reply from server */
        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = read(sock_fd, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            printf("Server disconnected.\n");
            break;
        }
        printf("Server: %s", buffer);

        if (strncmp(buffer, "quit", 4) == 0) break;
    }

    close(sock_fd);
    printf("Client closed.\n");
    return 0;
}
