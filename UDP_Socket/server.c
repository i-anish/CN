#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int client_struct_length = sizeof(client_addr);
    int bytes_received, bytes_sent;

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Create UDP socket:
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

    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    printf("Waiting for client messages...\n\n");

    // Exchange multiple messages:
    while(1) {

        memset(client_message, '\0', sizeof(client_message));
        memset(server_message, '\0', sizeof(server_message));

        // Receive client's message:
        bytes_received = recvfrom(socket_desc, client_message,
                                  sizeof(client_message) - 1, 0,
                                  (struct sockaddr*)&client_addr,
                                  &client_struct_length);

        if(bytes_received < 0) {
            printf("Couldn't receive\n");
            break;
        }

        client_message[bytes_received] = '\0';

        printf("\nReceived message from IP: %s and port: %i\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        printf("Msg from client: %s\n", client_message);
        printf("Bytes received by server: %d\n", bytes_received);

        // Check termination condition:
        if(strcmp(client_message, "exit") == 0) {
            strcpy(server_message, "Server disconnected");
            bytes_sent = sendto(socket_desc, server_message,
                                strlen(server_message), 0,
                                (struct sockaddr*)&client_addr,
                                client_struct_length);

            printf("Bytes sent by server: %d\n", bytes_sent);
            break;
        }

        // Respond to client:
        strcpy(server_message, client_message);

        bytes_sent = sendto(socket_desc, server_message,
                            strlen(server_message), 0,
                            (struct sockaddr*)&client_addr,
                            client_struct_length);

        if(bytes_sent < 0) {
            printf("Can't send\n");
            break;
        }

        printf("Bytes sent by server: %d\n", bytes_sent);
    }

    // Close the socket:
    close(socket_desc);

    return 0;
}

