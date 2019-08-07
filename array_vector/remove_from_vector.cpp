#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>


// vec.erase(vec.begin() + index);


using namespace std;

int main()
{
    std::vector<int> vec = {11,12,13,14,15,16,17,18,19,20};
    
//     vec.erase(vec.begin() + 1);
//     
//     for (auto i:vec)
//         cout << i << endl;
//     
    vec.erase(vec.begin() + 1, vec.begin() + 3);
    
    for (auto i:vec)
        cout << i << endl;

    return 0;
}
