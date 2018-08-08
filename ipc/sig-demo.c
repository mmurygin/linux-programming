#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signalno)
{
    printf("Called with %d\n", signalno);
}

int main()
{
    signal(SIGINT, signal_handler);

    daemon(0,1);

    printf("pid: %d\n", getpid());

    while(1);

    return 0;
}
