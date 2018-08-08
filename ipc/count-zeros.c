#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage ./count-zeros command args\n");
        return 1;
    }

    const char * cmd = argv[1];
    const char * args = argv[2];

    char * popencommand = malloc(strlen(cmd) + strlen(args) + 2);
    *popencommand = '\0';

    strcat(popencommand, cmd);
    strcat(popencommand, " ");
    strcat(popencommand, args);

    FILE * fd = popen(popencommand, "r");

    int zeros_count = 0;
    int c;
    while ((c = fgetc(fd)) != EOF)
    {
        if (c == '0')
        {
            ++zeros_count;
        }
    }

    printf("%d\n", zeros_count);

    free(popencommand);
    pclose(fd);

    return 0;
}
