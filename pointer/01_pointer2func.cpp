#include <iostream>

using namespace std;

/*------------------------------*/
void func(void (*f)(int))
{
    for (int ctr = 0; ctr < 5; ctr++)
    {
        (*f)(ctr);
    }
}
/*------------------------------*/
void print(int x)
{
    cout << x << endl;
}
/*------------------------------*/

int main()
{
    int x = 10;
    func(print);

    return 0;
}