#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;

int main()
{
    vector<double> vec(5, 2);
    constexpr double coefficient = 3.0;
    for (auto i : vec)
        cout << i << endl;

    std::transform(vec.begin(),
                   vec.end(),
                   vec.begin(),
                   std::bind(std::multiplies<double>(),
                             std::placeholders::_1, coefficient));

    for (auto i : vec)
        cout << i << endl;
}
