#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>

int main()
{
    std::random_device rd;
    std::mt19937 mt_rand(rd());
    std::normal_distribution<> dist(70, 10);

    std::map<int, int> hist;
    for (int n = 0; n < 10; ++n)
    {
        ++hist[std::round(dist(mt_rand))];
    }

    for (auto p : hist)
    {
        std::cout << std::fixed << std::setprecision(1) << std::setw(2)
                  << p.first << ' ' << std::string(p.second / 200, '*') << '\n';
    }
}
