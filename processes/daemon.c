#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid = getpid();

    printf("%d\n", pid);
    daemon(0, 0);

    while(1);

    return 0;
}
