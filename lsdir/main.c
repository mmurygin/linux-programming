#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void ls_dir(const char * name);

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Error: Please use syntax: lsdir dirname\n");
        return 1;
    }

    ls_dir(argv[1]);

    printf("Done\n");

    return 0;
}

void ls_dir(const char * name)
{
    printf("The content of dir '%s':\n", name);

    DIR *dir = opendir(name);

    struct dirent *dirent;
    while ((dirent = readdir(dir)) != NULL)
    {
        printf("%s\n", dirent->d_name);
    }

    closedir(dir);
}
