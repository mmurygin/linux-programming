#include <stdio.h>
#include "solution.h"

int main()
{
    int count = 0;
    char * str = "This is string";

    const int res1 = stringStat(str, 10, &count);
    printf("Len: %d, count: %d\n", res1, count);

    const int res2 = stringStat(str, 100, &count);
    printf("Len: %d, count: %d\n", res2, count);

    return 0;
}
