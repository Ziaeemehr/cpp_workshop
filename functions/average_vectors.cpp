#include <vector>
#include <numeric>
#include <iostream>

float average(std::vector<float> const &v)
{
    if (v.empty())
    {
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}

int main()
{
    std::vector<float> v{8, 4, 2, 7, 5};
    auto const a = average(v);
    std::cout << "average: " << a << "\n";
}


// compile:
// g++ -std=c++17 average_vectors.cpp
