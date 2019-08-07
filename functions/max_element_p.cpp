#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

int main()
{
    vector<int> v = {3, 7, 1, 9, 3};
    int a = *std::max_element(v.begin(), v.end());
    cout << a << endl;


}