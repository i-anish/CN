#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {

    int soc, addrlen;
    char buf[1024];
    struct sockaddr_in server;

    if (argc != 3) {
        printf("Usage: %s <IP Address> <Port Number>\n", argv[0]);
        exit(1);
    }

    soc = socket(AF_INET, SOCK_STREAM, 0);

    if (soc < 0) {
        perror("\nError in opening socket ... ");
        exit(1);
    }

    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(atoi(argv[2]));

    if (connect(soc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("\nError in connection ... ");
        exit(2);
    }

    do {
        printf("\nClient input : ");

        if (scanf(" %1023[^\n]", buf) != 1)
            break;

        write(soc, buf, strlen(buf) + 1);

        if (strcmp(buf, "bye") == 0)
            break;

        memset(buf, 0, sizeof(buf));

        if (read(soc, buf, sizeof(buf)) <= 0)
            break;

        printf("\nMessage from server :: %s\n", buf);

    } while (strcmp(buf, "bye") != 0);

    close(soc);

    return 0;
}

