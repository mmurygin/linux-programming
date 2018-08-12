#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_SIZE 1000

int * get_shm_ptr(key_t key);
int *crete_shm(const char *path, key_t * key);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage ./shared key1 key2");
        exit(1);
    }

    key_t key1 = atoi(argv[1]);
    key_t key2 = atoi(argv[2]);
    key_t res_key;

    int *mem_1 = get_shm_ptr(key1);
    int *mem_2 = get_shm_ptr(key2);
    int *res_mem = crete_shm(argv[0], &res_key);

    int i;
    for (i = 0; i < 100; i++)
    {
        *(res_mem + i) = *(mem_1 + i) + *(mem_2 + i);
    }

    printf("%d\n", res_key);

    return 0;
}

int * get_shm_ptr(key_t key)
{
    int shm_id = shmget(key, SHM_SIZE, 0666);
    if (shm_id == -1)
    {
        printf("Can not get shm_id for %d", key);
        exit(1);
    }

    return shmat(shm_id, NULL, SHM_RDONLY);
}

int * crete_shm(const char * path, key_t *key_ptr)
{
    *key_ptr = ftok(path, getpid());

    int shm_id = shmget(*key_ptr, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1)
    {
        printf("Can not get shm_id for %d", *key_ptr);
        exit(1);
    }

    return shmat(shm_id, NULL, 0);
}
