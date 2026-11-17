
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);
    int bytes_sent, bytes_received;

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Exchange multiple messages:
    while(1) {

        memset(client_message, '\0', sizeof(client_message));
        memset(server_message, '\0', sizeof(server_message));

        // Get input from the user:
        printf("\nEnter message (type exit to quit): ");
        fgets(client_message, sizeof(client_message), stdin);

        // Remove newline character:
        client_message[strcspn(client_message, "\n")] = '\0';

        // Send the message to server:
        bytes_sent = sendto(socket_desc, client_message,
                            strlen(client_message), 0,
                            (struct sockaddr*)&server_addr,
                            server_struct_length);

        if(bytes_sent < 0) {
            printf("Unable to send message\n");
            break;
        }

        printf("Bytes sent by client: %d\n", bytes_sent);

        // Receive the server's response:
        bytes_received = recvfrom(socket_desc, server_message,
                                  sizeof(server_message) - 1, 0,
                                  (struct sockaddr*)&server_addr,
                                  &server_struct_length);

        if(bytes_received < 0) {
            printf("Error while receiving server's msg\n");
            break;
        }

        server_message[bytes_received] = '\0';

        printf("Server's response: %s\n", server_message);
        printf("Bytes received by client: %d\n", bytes_received);

        // Check termination condition:
        if(strcmp(client_message, "exit") == 0) {
            break;
        }
    }

    // Close the socket:
    close(socket_desc);

    return 0;
}

