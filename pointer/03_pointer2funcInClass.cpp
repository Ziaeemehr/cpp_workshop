#include <iostream>

class myclass;
using namespace std;
typedef int (myclass::*myFuncType)(int, int);

class myclass
{
private:
    int a;
    int b;

public:
    ~myclass(){};

    int add(const int x1, const int x2)
    {
        return (x1 + x2);
    }
    int mult(const int x1, const int x2)
    {
        return (x1 * x2);
    }

    int combiner(const int x1, const int x2, myFuncType func)
    {
        return (this->*func)(x1, x2);
    }
    void useAdd(const int x1, const int x2)
    {
        cout << "using inside the class " << combiner(x1, x2, &myclass::add) << endl;
    }
};
/*------------------------------*/

int main()
{
    myclass classi;
    cout << classi.add(4, 5) << endl;

    myFuncType ptrName = &myclass::add;
    cout << (classi.*ptrName)(4, 5) << endl;
    cout << classi.combiner(4, 5, &myclass::add) << endl;

    classi.useAdd(4, 5);

    return 0;
}