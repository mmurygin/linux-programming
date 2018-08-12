#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUF_SIZE 100

int get_pipe_fd(const char * pipe_name);

int main(int argc, char const *argv[])
{
    printf("Start\n");
    if (argc != 3)
    {
        printf("Usage ./multi pipe_1 pipe_2");
        exit(1);
    }


    int pipe_1_fd = get_pipe_fd(argv[1]);
    int pipe_2_fd = get_pipe_fd(argv[2]);

    printf("Got descriptors %d %d\n", pipe_1_fd, pipe_2_fd);

    char * buf = malloc(BUF_SIZE + 1);
    fd_set fd_set;
    FD_ZERO(&fd_set);

    while (1)
    {
        FD_SET(pipe_1_fd, &fd_set);
        FD_SET(pipe_2_fd, &fd_set);

        if (select(pipe_2_fd + 1, &fd_set, NULL, NULL, NULL) <= 0)
        {
            break;
        }

        if (FD_ISSET(pipe_1_fd, &fd_set))
        {
            ssize_t size = read(pipe_1_fd, buf, BUF_SIZE);
            buf[size] = '\0';
        }

        if (FD_ISSET(pipe_2_fd, &fd_set))
        {
            ssize_t size = read(pipe_2_fd, buf, BUF_SIZE);
            buf[size] = '\0';
        }

        printf("%s", buf);
    }

    free(buf);
    close(pipe_1_fd);
    close(pipe_2_fd);

    return 0;
}

int get_pipe_fd(const char *pipe_name)
{
    int fd = open(pipe_name, O_RDWR);

    if (fd == -1)
    {
        printf("Can not get descriptor for file %s\n", pipe_name);
        exit(1);
    }

    return fd;
}
