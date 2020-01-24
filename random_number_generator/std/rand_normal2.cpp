#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>

int main()
{
    std::random_device rd;
    std::mt19937 mt_rand(rd());
    std::normal_distribution<> dist(0, 1);
    size_t N = 10000;
    std::vector<double> r(N);

    for (int n = 0; n < N; ++n)
    {
        r[n] = dist(mt_rand);
    }
    for (auto i : r)
        std::cout << i << std::endl;
}