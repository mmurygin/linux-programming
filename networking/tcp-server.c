#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage ./tcp-server port-name\n");
        return -1;
    }

    struct sockaddr_in local;

    int port = atoi(argv[1]);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;

    bind(server_socket, (struct sockaddr *) &local, sizeof(local));
    listen(server_socket, 5);

    printf("TCP server is listening at port %d\n", port);

    while (1)
    {
        printf("Waiting for connection\n");
        int client_socket = accept(server_socket, NULL, NULL);
        printf("Got connection %d\n", client_socket);

        char buf[BUFSIZ];
        ssize_t bytes_read = read(client_socket, buf, BUFSIZ);
        buf[bytes_read] = '\0';
        printf("%s", buf);
        close(client_socket);
    }

    return 0;
}
