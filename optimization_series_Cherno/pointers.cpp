#include <iostream>
#include <cstring>

#define LOG(x) std::cout << x << std::endl;

int main()
{
    int var = 7;
    int* ptr = &var;
    *ptr = 9;
    LOG(var);

    char* buffer = new char[8];
    memset(buffer, 0, 8);

    delete [] buffer;


    char** ptr1 = & buffer;
    return 0;
}
