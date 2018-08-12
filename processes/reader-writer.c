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

static int shared_var = 0;

void *reader(void *param);
void *writer(void *param);

static int resource_counter = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;    /* mutex lock for buffer */
pthread_cond_t c_readers = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t c_writers = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

int main(int argc, const char * argv[])
{
    int s;
    if (argc != 3)
    {
        fprintf(stderr, "Usabe ./rw readers-count writers-count\n");
        return 1;
    }

    srand(time(NULL));

    int readers_count = atoi(argv[1]);
    int writers_count = atoi(argv[2]);
    int num_threads = readers_count + writers_count;

    pthread_t * threads = calloc(num_threads, sizeof(pthread_t));

    if (threads == NULL) handle_error("calloc");

    puts("Memory allocated");

    for (int i = 0; i < readers_count; i++)
    {
        s = pthread_create(&threads[i], NULL, reader, NULL);

        if (s != 0) handle_error_en(s, "pthread_create");
    }

    printf("%d readers are created\n", readers_count);

    for (int i = 0; i < writers_count; i++)
    {
        s = pthread_create(&threads[i + readers_count], NULL, writer, NULL);

        if (s != 0) handle_error_en(s, "pthread_create");
    }

    printf("%d writers are created\n", writers_count);

    void *res;
    for (int i = 0; i < num_threads; i++)
    {
        s = pthread_join(threads[i], &res);

        if (s != 0) handle_error_en(s, "pthread_join");

        printf("Joined with thread %lu\n", threads[i]);
    }

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c_readers);
    pthread_cond_destroy(&c_writers);

    free(threads);

    return 0;
}

void read_shared()
{
    pthread_mutex_lock(&m);
    while (resource_counter == -1)
    {
        pthread_cond_wait(&c_readers, &m);
    }
    ++resource_counter;
    pthread_mutex_unlock(&m);

    printf("RS(%lu)\n", pthread_self());
    usleep(rand() % 10000);
    printf("RF(%lu): %d\n", pthread_self(), shared_var);

    pthread_mutex_lock(&m);
    --resource_counter;

    if (resource_counter == 0)
    {
        pthread_cond_signal(&c_writers);
    }

    pthread_mutex_unlock(&m);
}

void write_shared()
{
    pthread_mutex_lock(&m);
    while (resource_counter != 0)
    {
        pthread_cond_wait(&c_writers, &m);
    }
    resource_counter = -1;
    pthread_mutex_unlock(&m);

    printf("WS(%lu)\n", pthread_self());
    usleep(rand() % 10000);
    ++shared_var;
    printf("WF(%lu): %d\n", pthread_self(), shared_var);

    pthread_mutex_lock(&m);
    resource_counter = 0;
    pthread_cond_broadcast(&c_readers);
    pthread_cond_signal(&c_writers);
    pthread_mutex_unlock(&m);
}

void * reader(void *param)
{
    for (int i = 0; i < 5; i++)
    {
        read_shared();
        usleep(rand() % 10000);
    }

    return 0;
}

void *writer(void *param)
{
    for (int i = 0; i < 5; i++)
    {
        write_shared();
        usleep(rand() % 10000);
    }

    return 0;
}
