#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dlfcn.h>

int (*fun)(int);

bool init_library(const char * lib_name, const char * fun_name)
{
    void *hdl = dlopen(lib_name, RTLD_LAZY);

    if (hdl == NULL)
    {
        return false;
    }

    fun = (int (*)(int))dlsym(hdl, fun_name);

    if (fun == NULL)
    {
        return false;
    }

    return true;
}

int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        printf("The syntax is the following: ./app library function arg");
        return 1;
    }

    const char * lib_name = argv[1];
    const char * fun_name = argv[2];
    const int fun_arg = atoi(argv[3]);

    if (init_library(lib_name, fun_name))
    {
        int result = fun(fun_arg);
        printf("%d\n", result);
    }
    else
    {
        printf("Can not load library or function: %s, %s\n", lib_name, fun_name);
        return 1;
    }

    return 0;
}
