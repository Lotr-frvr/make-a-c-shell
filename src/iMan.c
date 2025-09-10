#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

void iMan(char* args[]) {
    if (args[1] == NULL) {
        printf("iMan invalid command, no args\n");
        return;
    }

    char* command = args[1]; // take only the first argument
    char host[] = "man.he.net";
    char request[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    struct hostent *server;
    struct sockaddr_in server_addr;
    int sockfd, bytes_received;
    int header_done = 0;
    char *header_end;

    // get server IP
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host\n");
        return;
    }

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    // fill server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP port
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

    // connect
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return;
    }

    // prepare GET request
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=0 HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             command, host);

    // send request
    if (write(sockfd, request, strlen(request)) < 0) {
        perror("write");
        close(sockfd);
        return;
    }

    // read response and skip HTTP headers
    while ((bytes_received = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_received] = '\0';

        if (!header_done) {
            header_end = strstr(buffer, "\r\n\r\n"); // HTTP headers end
            if (header_end) {
                header_end += 4; // skip "\r\n\r\n"
                printf("%s", header_end);
                header_done = 1;
            }
        } else {
            printf("%s", buffer);
        }
    }

    if (bytes_received < 0) {
        perror("read");
    }

    close(sockfd);
}
