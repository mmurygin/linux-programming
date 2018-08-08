#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static int sig1 = 0;
static int sig2 = 0;

void sig1_handler(int signalno)
{
    ++sig1;
}

void sig2_handler(int signalno)
{
    ++sig2;
}

void term_handler(int signalno)
{
    printf("%d %d\n", sig1, sig2);
    exit(0);
}

int main()
{
    signal(SIGUSR1, sig1_handler);
    signal(SIGUSR2, sig2_handler);
    signal(SIGTERM, term_handler);

    while (1);

    return 0;
}
