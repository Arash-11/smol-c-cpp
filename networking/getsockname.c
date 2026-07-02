/*
Using getsockname() to show that, if listen() is called on a TCP socket
without first calling bind(), the socket is assigned an ephemeral port.
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(void) {
    for (int i = 0; i < 3; i++) {
        int sockfd = socket(PF_INET, SOCK_STREAM, 0);
        // skip error checking for socket()

        listen(sockfd, SOMAXCONN);
        // skip error checking for listen()

        struct sockaddr sa;
        socklen_t addr_len = sizeof(sa);
        getsockname(sockfd, &sa, &addr_len);
        // skip error checking for getsockname()

        char host[NI_MAXHOST];
        char port[NI_MAXSERV];
        getnameinfo(&sa, sa.sa_len, host, sizeof(host), port, sizeof(port), 0);
        // skip error checking for getnameinfo()

        printf("host = %s\n", host);
        printf("port = %s\n", port);
        printf("\n");

        close(sockfd);
    }

    return 0;
}

/*
Example output:
    host = 0.0.0.0
    port = 61038

    host = 0.0.0.0
    port = 61039

    host = 0.0.0.0
    port = 61040
*/
