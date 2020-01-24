#include <vector>
#include <iostream>

typedef std::vector<int> dim1;
using std::cout;
using std::endl;

class myclass
{
public:
    dim1 x;
    void set_variable(const dim1 &x)
    {
        this->x = x;
    }
    void get_variable()
    {
        x[0] += 10;
        for (auto i : x)
            cout << i << " ";
        cout << endl;
    }
};

int main()
{
    myclass mc;
    dim1 x{10, 10};
    mc.set_variable(x);
    mc.get_variable();
    for (auto i : x)
        cout << i << " ";
    cout << endl;

    return 0;
}