#include <iostream>
#include <vector>


template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T>>& v) {
    std::size_t total_size = 0;
    for (const auto& sub : v)
        total_size += sub.size(); // I wish there was a transform_accumulate
    std::vector<T> result;
    result.reserve(total_size);
    for (const auto& sub : v)
        result.insert(result.end(), sub.begin(), sub.end());
    return result;
}

int main()
{
    std::vector<std::vector<double> > v2 = {{1.,2.,3.},{4.,5.}};
    auto v2f = flatten(v2);
    std::cout << v2.size() << " " << v2[0].size() << " " << v2f.size() << "\n"; 
    for (auto i : v2f)
        std::cout << i << " ";
    std::cout << "\n";
}