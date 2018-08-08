#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void signal_handler(int signalno)
{
    exit(0);
}

int main()
{
    signal(SIGURG, signal_handler);

    daemon(0, 1);

    printf("%d\n", getpid());

    usleep(50000000);

    return 0;
}
