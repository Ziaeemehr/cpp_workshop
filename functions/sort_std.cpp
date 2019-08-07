#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

using std::endl;
using std::cout;
 
int main()
{
    std::array<int, 10> s = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3}; 
 
    // sort using the default operator<
    std::sort(s.begin(), s.end());
    for (auto a : s) {
        std::cout << a << " ";
    }   
    std::cout << '\n';
    cout <<s[s.size()-1]<< "\t" << s[s.size()-2]<<endl;
 
}
