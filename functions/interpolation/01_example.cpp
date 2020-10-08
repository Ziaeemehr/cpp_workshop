#include <iostream>
#include <cmath>

// Precise method, which guarantees v = v1 when t = 1.
// from wikipedia "linear interpolation"

double lerp(double v0, double v1, double t)
{
    return (1 - t) * v0 + t * v1;
}
float lerp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}


int main()
{
    float a = 10.0f, b = 20.0f;

    std::cout << "a=" << a << ", "
              << "b=" << b << '\n'
              << "mid point=" << lerp(a, b, 0.5f) << '\n'
              << std::boolalpha << (a == lerp(a, b, 0.0f)) << ' '
              << std::boolalpha << (b == lerp(a, b, 1.0f)) << '\n';
    
    double a1 = 10.0, b1 = 20.0;

    std::cout << "a1=" << a << ", "
              << "b1=" << b << '\n'
              << "mid point=" << lerp(a1, b1, 0.5) << '\n'
              << std::boolalpha << (a1 == lerp(a1, b1, 0.0)) << ' '
              << std::boolalpha << (b1 == lerp(a1, b1, 1.0)) << '\n';
}