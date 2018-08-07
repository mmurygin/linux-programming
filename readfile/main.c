#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 100

int main(int argc, char const *argv[])
{
    char buf[BUF_SIZE + 1];
    size_t sz = 0;

    if (argc != 2)
    {
        printf("Usage: ./readfile filename\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);

    if (fd < 0)
    {
        perror("Error trying to open file: ");
        return 1;
    }

    printf("fd=%d\n", fd);

    while ((sz = read(fd, buf, BUF_SIZE)) > 0)
    {
        buf[sz] = '\0';
        printf("%s", buf);
    }

    if (sz == -1)
    {
        perror("Error reading file: ");
        return 1;
    }

    close(fd);

    return 0;
}
