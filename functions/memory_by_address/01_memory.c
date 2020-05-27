#include <stdio.h>

void print_bits_int(int num)
{
    for (int bit = 0; bit < (sizeof(int) * 8); bit++)
    {
        printf("%i ", num & 0x01);
        num = num >> 1;
    }
    printf("\n");
}

void print_bits_char(char num)
{
    for (int bit = 0; bit < (sizeof(char) * 8); bit++)
    {
        printf("%i ", num & 0x01);
        num = num >> 1;
    }
    printf("\n");
}

int main()
{
    int a = 1;
    float b = 2.0;
    char c = 'a';

    printf("Address of a:  %p\n", &a);
    printf("Address of b:  %p\n", &b);
    printf("Address of c:  %p\n", &c);

    // Declare and Initialize pointers
    int *ptr_a = &a;
    float *ptr_b = &b;
    char *ptr_c = &c;

    //Printing address by using pointers
    printf("Address of a: %p\n", ptr_a);
    printf("Address of b: %p\n", ptr_b);
    printf("Address of c: %p\n", ptr_c);

    int var = 2;
    // print_bits_int(var);
    // print_bits_char(c);

    int memory = 2;
    for (int i = 0; i < 32; i++)
    {
        printf("%d ", memory >> i & 1);
    }
    printf("\n");

    return 0;
}

// #include <stdio.h>

// int main(void)
// {
//     // pointer to int (4bytes) that points to memory address 0x611268
//     int *address = (int *)0x611268;

//     printf("Memory address is: 0x%x\n", address);

//     // Note that this address should exist on your process' memory or
//     // the line below will cause a Segmentation Fault
//     *address = 0xdead; //assign a value to that address

//     printf("Content of that address is: 0x%x\n", *address);

//     fprintf(stderr, "myVar contains %p\n", (void*)myVar);
//     return 0;
// }