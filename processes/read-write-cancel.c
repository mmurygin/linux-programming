#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

static int read_count = 0;
static int write_count = 0;
static int cancel_count = 0;
static unsigned long started;

static int pending_cancel = 0;
static int pending_read = 0;
static int prending_writes = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;       /* mutex lock for buffer */
pthread_cond_t c_can_read = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t c_can_write = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */
pthread_cond_t c_can_cancel = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

void * reader(void * param);
void * writer(void * param);
void * cancel(void * param);

int main(int argc, const char * argv[])
{
    started = time(NULL);
    int s;
    if (argc != 4)
    {
        fprintf(stderr, "Usabe ./rw readers-count writers-count cancel-count\n");
        return 1;
    }

    srand(time(NULL));

    int readers_count = atoi(argv[1]);
    int writers_count = atoi(argv[2]);
    int cancel_threads_count = atoi(argv[3]);
    int num_threads = readers_count + writers_count + cancel_threads_count;

    pthread_t *threads = calloc(num_threads, sizeof(pthread_t));

    if (threads == NULL)
        handle_error("calloc");

    puts("Memory allocated");

    for (int i = 0; i < readers_count; i++)
    {
        s = pthread_create(&threads[i], NULL, reader, NULL);

        if (s != 0)
            handle_error_en(s, "pthread_create");
    }

    printf("%d readers are created\n", readers_count);

    for (int i = 0; i < writers_count; i++)
    {
        s = pthread_create(&threads[i + readers_count], NULL, writer, NULL);

        if (s != 0)
            handle_error_en(s, "pthread_create");
    }

    printf("%d writers are created\n", writers_count);

    for (int i = 0; i < cancel_threads_count; i++)
    {
        s = pthread_create(&threads[i + readers_count + writers_count], NULL, cancel, NULL);

        if (s != 0)
            handle_error_en(s, "pthread_create");
    }

    printf("%d cancels are created\n", writers_count);

    void *res;
    for (int i = 0; i < num_threads; i++)
    {
        s = pthread_join(threads[i], &res);

        if (s != 0)
            handle_error_en(s, "pthread_join");

        printf("Joined with thread %lu\n", threads[i]);
    }

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c_can_read);
    pthread_cond_destroy(&c_can_write);
    pthread_cond_destroy(&c_can_cancel);

    printf("C: %d, R: %d, W:%d\n", cancel_count, read_count, write_count);

    free(threads);

    return 0;
}

void read_shared()
{
    pthread_mutex_lock(&m);

    while (pending_cancel || pending_read)
    {
        pthread_cond_wait(&c_can_read, &m);
    }

    ++pending_read;

    pthread_mutex_unlock(&m);

    printf("Read shared\n");
    ++read_count;
    usleep(rand() % 10000);
    printf("Finish reading\n");

    pthread_mutex_lock(&m);
    --pending_read;
    if (pending_cancel)
    {
        pthread_cond_signal(&c_can_cancel);
    }
    else if (pending_read)
    {
        pthread_cond_signal(&c_can_cancel);
    }
    pthread_cond_signal(&c_can_write);
    pthread_mutex_unlock(&m);
}

void write_shared()
{
    pthread_mutex_lock(&m);

    while (pending_cancel || pending_read)
    {
        pthread_cond_wait(&c_can_write, &m);
    }

    printf("Write shared\n");
    ++write_count;
    usleep(rand() % 10000);
    printf("Finish writing\n");

    pthread_mutex_unlock(&m);
}

void cancel_shared()
{
    pending_cancel = 1;
    pthread_mutex_lock(&m);

    printf("Cancel shared\n");
    ++cancel_count;
    usleep(rand() % 10000);
    printf("Finish canceling\n");

    pthread_mutex_unlock(&m);

    pending_cancel = 0;
    pthread_cond_signal(&c_can_read);
    pthread_cond_signal(&c_can_write);
}

void *reader(void *param)
{
    while(time(NULL) - started < 2)
    {
        read_shared();
        usleep(rand() % 10000);
    }

    return 0;
}

void *writer(void *param)
{
    while(time(NULL) - started < 2)
    {
        write_shared();
        usleep(rand() % 10000);
    }

    return 0;
}

void *cancel(void *param)
{
    while(time(NULL) - started < 2)
    {
        cancel_shared();
        usleep(rand() % 10000);
    }

    return 0;
}
