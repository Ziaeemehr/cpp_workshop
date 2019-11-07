#include <iostream>

using namespace std;

// A function that returns void and takes no arguments.
typedef void (*functiontype)();
typedef int (*functiontype2)(char);
typedef double (*functiontype3)(const double, const double);

int dosomethingwithchar(char a);
double add(const double a, const double b);
double mult(const double a, const double b);
/*------------------------------*/
// function pointer as a parameter
// double combiner(double a, double b, double(*func)(double, double))
// {
//     return func(a, b);
// }
/*------------------------------*/

// function pointer as a parameter with a function pointer type
double combiner(double a, double b, functiontype3 func)
{
    return func(a, b);
}

/*------------------------------*/

int main()
{
    functiontype2 func2 = &dosomethingwithchar;
    int result = func2('a');

    cout << combiner(1, 2, add) << endl;
    cout << combiner(1, 2, mult)<< endl;

    return 0;
}
/*------------------------------*/
int dosomethingwithchar(char a) 
{ 
    return 1; 
}
/*------------------------------*/
double add(const double a, const double b)
{
    return (a + b);
}
double mult(const double a, const double b)
{
    return (a * b);
}
