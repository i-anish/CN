#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1

int main(void) {
    int soc, msgsock;
    socklen_t len;
    socklen_t addrlen;
    char buf[1024];

    struct sockaddr_in server, client;

    soc = socket(AF_INET, SOCK_STREAM, 0);

    if (soc == -1) {
        perror("Error in opening socket");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = 0;

    if (bind(soc, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Error in bind");
        close(soc);
        exit(2);
    }

    len = sizeof(server);

    if (getsockname(soc, (struct sockaddr *)&server, &len) == -1) {
        perror("Error in getting port");
        close(soc);
        exit(3);
    }

    printf("Socket has port # %hu\n", ntohs(server.sin_port));

    if (listen(soc, 5) == -1) {
        perror("Error in listen");
        close(soc);
        exit(4);
    }

    while (TRUE) {
        addrlen = sizeof(client);

        msgsock = accept(soc, (struct sockaddr *)&client, &addrlen);

        if (msgsock == -1) {
            perror("Error in accept");
            continue;
        }

        ssize_t n = read(msgsock, buf, sizeof(buf) - 1);

        if (n > 0) {
            buf[n] = '\0';
            printf("Message from client:\n%s\n", buf);
        } else if (n == 0) {
            printf("Client closed the connection without sending data.\n");
        } else {
            perror("Error in read");
        }

        close(msgsock);
    }

    close(soc);
    return 0;
}
