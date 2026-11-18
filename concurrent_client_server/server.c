#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TRUE 1

int main() {
    int soc, chpid;
    socklen_t len;
    char buf[1024], buf1[1024];
    struct sockaddr_in server, client;
    int msgsock;
    socklen_t addrlen = sizeof(client);

    soc = socket(AF_INET, SOCK_STREAM, 0);

    if (soc < 0) {
        perror("\nError in opening socket ... ");
        exit(1);
    }

    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = 0;

    if (bind(soc, (struct sockaddr *)&server, sizeof(server))) {
        perror("\nError in bind ... ");
        exit(2);
    }

    len = sizeof(server);

    if (getsockname(soc, (struct sockaddr *)&server, &len)) {
        perror("\nError in getting port... ");
        exit(3);
    }

    printf("\nSocket has port # %hd \n", ntohs(server.sin_port));

    /* Create a connection queue */
    listen(soc, 5);

    signal(SIGCHLD, SIG_IGN);

    do {
        msgsock = accept(soc, (struct sockaddr *)&client, &addrlen);

        if (msgsock == -1) {
            perror("\nError in accept... ");
            continue;
        }
        else {
            if ((chpid = fork()) == 0) {

                /* CHILD PROCESS */
                close(soc);

                do {
                    memset(buf, 0, sizeof(buf));
                    memset(buf1, 0, sizeof(buf1));

                    if (read(msgsock, buf, sizeof(buf)) <= 0)
                        break;

                    printf("\nMessage from client :: \n %s\n", buf);

                    printf("\nServer input : ");
                    scanf(" %1023[^\n]", buf1);

                    write(msgsock, buf1, strlen(buf1) + 1);

                } while (strcmp(buf, "bye") != 0);

                close(msgsock);
                exit(0);
            }
            else if (chpid > 0) {
                /* PARENT PROCESS */
                close(msgsock);
            }
            else {
                perror("\nError in fork ... ");
                close(msgsock);
            }
        }

    } while (TRUE);

    close(soc);
    return 0;
}

