#include <stdio.h>
#include <stdlib.h>

int get_ppid(int cur_id);

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./process-parents pid\n");
        return 1;
    }

    int pid = atoi(argv[1]);
    if (pid <= 0)
    {
        printf("Pid should be positive integer\n");
        return 1;
    }

    while (pid > 1)
    {
        printf("%d\n", pid);
        pid = get_ppid(pid);
    }

    printf("%d\n", pid);

    return 0;
}

int get_ppid(int cur_id)
{
    char path[50];

    snprintf(path, sizeof(path), "/proc/%d/status", cur_id);

    FILE *stat_file = fopen(path, "r");

    if (stat_file == NULL)
    {
        printf("Can not open file %s\n", path);
        exit(1);
    }

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

    fclose(stat_file);

    return ppid;
}
