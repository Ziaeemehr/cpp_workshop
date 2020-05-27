#include <stdio.h>

int main()
{
    int a = 2;
    char c = 'a';

    printf("Address of a:  %p\n", &a);
    printf("Address of c:  %p\n", &c);

    // to print the bites in integer from left to right
    // 2 in binary is : 10

    for (int i = 0; i < (sizeof(int) * 8); i++)
    {
        printf("%d ", a >> i & 1);
    }
    printf("\n");

    for (int i = 0; i < (sizeof(char) * 8); i++)
    {
        printf("%d ", c >> i & 1);
    }
    printf("\n");
    
    // to convert integer to char in C
    char c1 = a;
    for (int i = 0; i < (sizeof(char) * 8); i++)
    {
        printf("%d ", c1 >> i & 1);
    }
    printf("\n");

    printf("Address of c1:  %p\n", &c1);

    return 0;
}
