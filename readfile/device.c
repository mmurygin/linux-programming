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

    sz = read(fd, buf, BUF_SIZE);
    printf("Read done\n");

    if (sz == -1)
    {
        perror("Error reading file: ");
        return 1;
    }
    else if (sz == 0)
    {
        printf("EOF is reached");
    }
    else
    {
        buf[sz] = '\0';
        int all_zeros = 1;
        for (int i = 0; i < sz; i++)
        {
            if (buf[i] != '\0')
            {
                all_zeros = 0;
                break;
            }
        }


        printf("Read %zu bytes, allZeros: %d\n", sz, all_zeros);
    }

    close(fd);

    return 0;
}
