#include <stdio.h>
#include <stdlib.h>
/* function declaration */
double getAverage(int *arr, int size);

int main()
{

    /* an int array with 5 elements */
    // int balance[5] = {1000, 2, 3, 17, 50};
    int *balance;
    balance = (int *)malloc((5) * sizeof(int));
    balance[0] = 1000;
    balance[1] = 2;
    balance[2] = 3;
    balance[3] = 17;
    balance[4] = 50;

    double avg;

    /* pass pointer to the array as an argument */
    avg = getAverage(balance, 5);

    /* output the returned value  */
    printf("Average value is: %f\n", avg);
    return 0;
}

double getAverage(int *arr, int size)
{

    int i, sum = 0;
    double avg;

    for (i = 0; i < size; ++i)
    {
        sum += arr[i];
    }

    avg = (double)sum / size;
    return avg;
}