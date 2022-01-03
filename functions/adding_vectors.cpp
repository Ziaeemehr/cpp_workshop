#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

void add(const vector<double> &a, const vector<double> &b, vector<double> &c)
{
    transform(a.begin(), a.end(), b.begin(), c.begin(),
              [](double a, double b)
              { return a + b; });
}

int main()
{
    std::vector<double> a = {1, 2, 3, 4, 5}, b = {9, 8, 7, 6, 5}, c(5);
    add(a, b, c);
    for (auto x : c)
        std::cout << x << ' ';
    std::cout << std::endl;

    add(a, b, a);
    for (auto x : a)
        std::cout << x << ' ';
    std::cout << std::endl;

    return 0;
}

// #include <iostream>
// #include <vector>
// #include <exception>

// template <typename T>
// std::vector<T> &operator+=(std::vector<T> &lhs, const std::vector<T> &rhs)
// {
//     typedef std::vector<T>::size_type size_type;
//     if (lhs.size() != rhs.size())
//         throw std::length_error("vectors must be same size to add");

//     for (size_type i = 0; i < lhs.size(); ++i)
//         lhs[i] += rhs[i];
//     return lhs;
// }

// template <typename T>
// std::vector<T> operator+(std::vector<T> lhs, const std::vector<T> &rhs)
// {
//     typedef std::vector<T>::size_type size_type;
//     if (lhs.size() != rhs.size())
//         throw std::length_error("vectors must be same size to add");
//     return lhs += rhs;
// }

// int main()
// {
//     std::vector<double> a = {1, 2, 3, 4, 5}, b = {9, 8, 7, 6, 5}, c;
//     c = a + b;
//     for (auto x : c)
//         std::cout << x << ' ';
//     return 0;
// }