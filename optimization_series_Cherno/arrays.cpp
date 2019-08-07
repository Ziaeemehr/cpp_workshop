#include <iostream>

#define LOG(X) std::cout<< X << std::endl;

int main()
{
    int N = 5;
    int* a = new int[N];
    int b[N];

    for (int i=0; i<N; i++)
    {
        a[i] = 2;
        b[i] = 3;
    }
    

    int count = sizeof(b)/sizeof(int);
    LOG(count);

    delete[] a;



    return 0;
}