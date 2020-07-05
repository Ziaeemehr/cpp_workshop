// https://en.cppreference.com/w/cpp/algorithm/min_element
#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v{3, 0, -4, 2, 5, 9};

    std::vector<int>::iterator result = std::min_element(v.begin(), v.end());
    std::cout << "min element at: " << std::distance(v.begin(), result) << "\n";

    std::cout << "min element at: " << std::distance(v.begin(), std::max_element(v.begin(), v.end()))
              << "\n";
    
    std::vector<double> vd{3.0, 0.0, -4.0, 2.0, 5.0, 9.0};
    std::cout << "min element at: " << std::distance(vd.begin(), std::min_element(vd.begin(), vd.end())) << "\n";
    std::cout << "min element at: " << std::distance(vd.begin(), std::max_element(vd.begin(), vd.end()))
              << "\n";
}


// output:
// min element at: 2
// min element at: 5
// min element at: 2
// min element at: 5
