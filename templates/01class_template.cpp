#include <iostream>
using namespace std;

template <class T>
class mypair {
    T a, b;
    public:
    mypair (T first, T second)
    {
        a = first; 
        b = second;
    }
    T getmax();
};

template <class T>
T mypair<T>::getmax()
{
    return (a>b?a:b);
}

int main() {
    mypair<int> myobject(100,75);
    cout << myobject.getmax() << endl;
    return 0;
}
