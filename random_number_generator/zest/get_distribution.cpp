#include <random>
#include <iostream>
#include <cmath>

// include Zest and import it into the global namespace
#include "include/zest.hpp"
// using namespace zest;

double average(const double *, const size_t);
double standardDeviation(const double *, const size_t);
//-----------------------------------------------------------------//
int main()
{
    std::mt19937 urbg;

    // zest::Normal normal_dist{/*mean =*/-2, /*stddev =*/1.5};
    // Ziggurat<zest::Normal, std::mt19937> generate{normal_dist};

    zest::Gamma gamma_dist {/*shape =*/ 2.5, /*scale =*/ 3};          // default is shape = 1.0, scale = 1.0
    Ziggurat<zest::Gamma, std::mt19937> generate {gamma_dist};

    size_t N = 10000;
    double arr[N];
    double aver = 0.0;
    double stdv = 0.0;

    FILE *outputFile = fopen("r.txt", "w");

    for (size_t i = 0; i < N; ++i)
    {
        arr[i] = generate(urbg);
        fprintf(outputFile, "%18.9f\n", arr[i]);
    }

    fclose(outputFile);

    std::cout << "AVERAGE : " << average(arr, N) << "\n";
    std::cout << "STD DEVIATION : " << standardDeviation(arr, N) << "\n";



    return 0;
}
//-----------------------------------------------------------------//
double average(const double *arr, const size_t N)
{
    double average_value = 0.0;
    for (size_t i = 0; i < N; ++i)
        average_value += arr[i];

    return average_value / float(N);
}
//-----------------------------------------------------------------//
double standardDeviation(const double *vec, const size_t N)
{

    double m = average(vec, N);
    double s = 0.0;

    for (size_t i = 0; i < N; ++i)
    {
        double tmp = vec[i] - m;
        s += (tmp * tmp);
    }
    s /= float(N);

    return sqrt(s);
}
