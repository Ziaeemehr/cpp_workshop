
#include <ctime>
#include <random>
#include <chrono>
#include <iostream>

int main()
{
    std::default_random_engine eng;
    unsigned long int seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    eng.seed(seed);
    std::mt19937 rng(eng());
    // srand(time(0));

    std::uniform_real_distribution<double> uniform(0, 1);
    std::cout << uniform(eng) << std::endl;

    return 0;
}
