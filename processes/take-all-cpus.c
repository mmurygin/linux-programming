#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <time.h>

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

void * infinitive_loop(void *param);
static unsigned long started;
static int s;

int main(int argc, const char * argv[])
{
    started = time(NULL);

    int num_threads = get_nprocs();
    pthread_t *threads = calloc(num_threads, sizeof(pthread_t));

    for (int i = 0; i < num_threads; i++)
    {
        s = pthread_create(&threads[i], NULL, infinitive_loop, NULL);

        if (s != 0)
            handle_error_en(s, "pthread_create");
    }

    void *res;
    for (int i = 0; i < num_threads; i++)
    {
        s = pthread_join(threads[i], &res);

        if (s != 0)
            handle_error_en(s, "pthread_join");

        printf("Joined with thread %lu\n", threads[i]);
    }

    printf("Done in %lu\n", time(NULL) - started);
    free(threads);

    return 0;
}

void * infinitive_loop(void *param)
{
    unsigned long i = 0;
    double res = 2.0;
    while (i++ < 1000000000)
    {
        res = res * 1.111111111;

        if (res < 1)
        {
            i++;
        }
    }
}
