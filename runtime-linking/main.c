#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>

void (*hello)(const char *);

bool init_library() {
    void *hdl = dlopen("./libHello.so", RTLD_LAZY);

    if (hdl == NULL)
    {
        return false;
    }

    hello = (void (*)(const char *))dlsym(hdl, "hello");

    if (hello == NULL)
    {
        return false;
    }

    return true;
}

int main()
{
    if (init_library())
    {
        hello("Max");
    }
    else
    {
        printf("Library was not loaded\n");
        return 1;
    }

    return 0;
}
