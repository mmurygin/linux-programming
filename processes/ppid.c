#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t pid = getpid();

    char path[50];

    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE * stat_file = fopen(path, "r");

    int ppid = 0;

    char stat_str[100];
    while (fgets(stat_str, sizeof(stat_str), stat_file) != NULL)
    {
        sscanf(stat_str, "PPid: %d", &ppid);
        if (ppid != 0)
        {
            break;
        }
    }

    printf("%d\n", ppid);

    fclose(stat_file);

    return 0;
}
