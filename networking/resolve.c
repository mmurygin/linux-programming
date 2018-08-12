#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, const char * argv[])
{
    const char * hostname = argv[1];

    struct hostent *hostent = gethostbyname(hostname);

    if (hostent == NULL)
    {
        printf("Can not get hostend\n");
        return 1;
    }

    int i = 0;
    while (hostent->h_addr_list[i] != NULL)
    {
        struct in_addr *in_addr = (struct in_addr *)hostent->h_addr_list[i];
        char *addr = inet_ntoa(*in_addr);
        printf("%s\n", addr);
        i++;
    }

    return 0;
}
