#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

namespace std {
    template <typename T>
    vector<T> operator+(const vector<T>& a, const vector<T>& b)
    {
        assert(a.size() == b.size());
        vector<T> result;
        result.reserve(a.size());
        transform(a.begin(), a.end(), b.begin(), 
                    back_inserter(result), plus<T>());
        return result;
    }
}

int main()
{
    std::vector<int> a {2,0,1,5,0};
    std::vector<int> b {0,0,1,3,5};

    // std::plus adds together its two arguments:
    auto res = std::vector<int>(5);
    std::transform(a.begin(), a.end(), b.begin(), res.begin(), std::plus<int>());
    
    for (auto it = res.begin() ; it != res.end(); ++it)
        std::cout << " " << *it;
    std::cout << "\n";
    
    vector<int> c = a+b;

    return 0;
}


