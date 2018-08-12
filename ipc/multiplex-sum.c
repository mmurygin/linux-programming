#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUF_SIZE 20
#define FILE_1_NAME "in1"
#define FILE_2_NAME "in2"

static char buf[BUF_SIZE];
static int sum = 0;

int get_pipe_fd(const char *pipe_name);
int read_and_sum(int fd);

int main(int argc, char const *argv[])
{
    int pipe_1_fd = get_pipe_fd(FILE_1_NAME);
    int pipe_2_fd = get_pipe_fd(FILE_2_NAME);

    fd_set fd_set;
    FD_ZERO(&fd_set);

    int pip_1_opened = 1;
    int pip_2_opened = 1;

    while (pip_1_opened || pip_2_opened)
    {
        FD_SET(pipe_1_fd, &fd_set);
        FD_SET(pipe_2_fd, &fd_set);

        if (select(pipe_2_fd + 1, &fd_set, NULL, NULL, NULL) <= 0)
        {
            break;
        }

        if (FD_ISSET(pipe_1_fd, &fd_set))
        {
            pip_1_opened = read_and_sum(pipe_1_fd);
        }

        if (FD_ISSET(pipe_2_fd, &fd_set))
        {
            pip_2_opened = read_and_sum(pipe_2_fd);
        }
    }

    printf("%d\n", sum);

    close(pipe_1_fd);
    close(pipe_2_fd);

    return 0;
}

int get_pipe_fd(const char *pipe_name)
{
    int fd = open(pipe_name, O_NONBLOCK | O_RDONLY);

    if (fd == -1)
    {
        printf("Can not get descriptor for file %s\n", pipe_name);
        exit(1);
    }

    return fd;
}

int read_and_sum(int fd)
{
    ssize_t size = read(fd, buf, BUF_SIZE);

    if (size <= 0)
    {
        return 0;
    }

    buf[size] = '\0';
    sum += atoi(buf);

    return 1;
}
