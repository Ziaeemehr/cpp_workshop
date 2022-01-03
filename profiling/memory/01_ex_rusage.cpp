#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>
#include <cstdio>
// #include <iostream>




long get_mem_usage()
{
    struct rusage myusage;

    getrusage(RUSAGE_SELF, &myusage);
    return myusage.ru_maxrss;
}

int main()
{
    long baseline = get_mem_usage();

    for (size_t i=0; i<100; ++i)
    {
        char * buffer;
        buffer = (char*) malloc(1024*1000);
        memset(buffer, 1, 1024*1000);
        sleep(1);
        printf("usage : %ld + %ld\n", baseline, get_mem_usage()-baseline);
        // free(buffer);
    }

    return 0;
}