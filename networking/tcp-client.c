#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage ./tcp-client port-name\n");
        return -1;
    }

    struct sockaddr_in local;

    int port = atoi(argv[1]);
    int s = socket(AF_INET, SOCK_STREAM, 0);

    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;

    int connect_result = connect(s, (struct sockaddr*) &local, sizeof(local));

    if (connect_result == -1)
    {
        fprintf(stderr, "Can not connect to socket. Error %d\n", errno);
        return errno;
    }

    printf("Send packet to port %d\n", port);

    char * buf = "Hello\n";
    write(s, buf, strlen(buf) + 1);
    close(s);

    return 0;
}
