#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 5000
#define TERMINATE_STRING "OFF\n"

struct sockaddr_in local;

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage ./udp port-name\n");
        return -1;
    }

    int port = atoi(argv[1]);

    int s = socket(AF_INET, SOCK_DGRAM, 0);

    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;

    int result = bind(s, (struct sockaddr*) &local, sizeof(local));
    if (result == -1)
    {
        fprintf(stderr, "Can not bind to port %d\n", port);
        return -1;
    }

    char buf[BUFFER_SIZE + 1];
    while (1)
    {
        ssize_t bytes_read = read(s, buf, BUFFER_SIZE);
        buf[bytes_read] = '\0';

        if (bytes_read <= 0)
        {
            continue;
        }

        if (strcmp(buf, TERMINATE_STRING) == 0)
        {
            break;
        }

        printf("%s\n", buf);
    }

    return 0;
}
