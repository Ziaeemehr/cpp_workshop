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
    int maxElementIndex = std::max_element(v.begin(),v.end()) - v.begin();
    
    cout << "max element " << a << " at " << maxElementIndex << endl;
    
    int b = *std::min_element(v.begin(), v.end());
    int minElementIndex = std::min_element(v.begin(),v.end()) - v.begin();
    cout << "min element " << b << " at " << minElementIndex << endl;

}
