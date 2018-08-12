#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

static int compare_chars(const void * c1, const void * c2)
{
    return c1 < c2;
}

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

    if (bind(server_socket, (struct sockaddr *)&local, sizeof(local)) == -1)
    {
        fprintf(stderr, "Can not bind to port %d\n", port);
        return errno;
    }

    if (listen(server_socket, 5) == -1)
    {
        fprintf(stderr, "Can not listen to port %d\n", port);
        return errno;
    }

    printf("TCP server is listening at port %d\n", port);

    while (1)
    {
        printf("Waiting for connection\n");
        int client_socket = accept(server_socket, NULL, NULL);
        printf("Got connection %d\n", client_socket);

        char buf[BUFSIZ];
        ssize_t bytes_read = recv(client_socket, buf, BUFSIZ-1, 0);
        buf[bytes_read] = '\0';

        qsort(buf, bytes_read, sizeof(char), compare_chars);

        send(client_socket, buf, bytes_read, 0);

        close(client_socket);
    }

    return 0;
}
