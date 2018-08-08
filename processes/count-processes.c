#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define _GNU_SOURCE
#define SEARCH_PROCESS_NAME "genenv"
#define PROC_DIR_NAME "/proc"

void get_process_name(const char * process_dir, char * proc_name);
bool isNumber(const char *s);

int main()
{
    DIR * proc_dir = opendir(PROC_DIR_NAME);

    if (proc_dir == NULL)
    {
        printf("Can not open dir " PROC_DIR_NAME);
        return 1;
    }

    int process_count = 0;

    struct dirent * dir_end;
    char * proc_name = malloc(255);
    while ((dir_end = readdir(proc_dir)) != NULL)
    {
        if (isNumber(dir_end->d_name))
        {
            get_process_name(dir_end->d_name, proc_name);
            if (strcmp(proc_name, SEARCH_PROCESS_NAME) == 0)
            {
                ++process_count;
            }
        }
    }

    free(proc_name);
    closedir(proc_dir);
    printf("%d\n", process_count);

    return 0;
}

bool isNumber(const char * s)
{
    int str_len = strlen(s);
    int i;
    for (i = 0; i < str_len; i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}

void get_process_name(const char * process_dir, char * proc_name)
{
    char path[50];

    snprintf(path, sizeof(path), "/proc/%s/status", process_dir);

    FILE *stat_file = fopen(path, "r");

    char stat_str[255];
    while (fgets(stat_str, sizeof(stat_str), stat_file) != NULL)
    {
        if (sscanf(stat_str, "Name: %s", proc_name) > 0)
        {
            break;
        }
    }

    fclose(stat_file);
}
