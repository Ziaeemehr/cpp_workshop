#include <vector>
#include <iostream>
#include <math.h>

using namespace std;
/*---------------------------------------------------------------------------*/
// the integration routine
template <typename Method, typename F, typename Float>
double integrate(F f, Float a, Float b, int steps, Method m)
{
    double s = 0;
    double h = (b - a) / steps;
    for (int i = 0; i < steps; ++i)
        s += m(f, a + h * i, h);
    return h * s;
}

// methods
class rectangular
{
  public:
    enum position_type
    {
        left,
        middle,
        right
    };
    rectangular(position_type pos) : position(pos) {}
    template <typename F, typename Float>
    double operator()(F f, Float x, Float h) const
    {
        switch (position)
        {
        case left:
            return f(x);
        case middle:
            return f(x + h / 2);
        case right:
            return f(x + h);
        }
    }

  private:
    const position_type position;
};

class trapezium
{
  public:
    template <typename F, typename Float>
    double operator()(F f, Float x, Float h) const
    {
        return (f(x) + f(x + h)) / 2;
    }
};

class simpson
{
  public:
    template <typename F, typename Float>
    double operator()(F f, Float x, Float h) const
    {
        return (f(x) + 4 * f(x + h / 2) + f(x + h)) / 6;
    }
};

// sample usage
double f(double x) { return x * x * x; }



/*---------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    // inside a function somewhere:
    double rl = integrate(f, 0.0, 1.0, 10, rectangular(rectangular::left));
    double rm = integrate(f, 0.0, 1.0, 10, rectangular(rectangular::middle));
    double rr = integrate(f, 0.0, 1.0, 10, rectangular(rectangular::right));
    double t = integrate (f, 0.0, 1.0, 10, trapezium());
    double s = integrate (f, 0.0, 1.0, 10, simpson());

    cout << "r:left\t\t" << rl << endl;
    cout << "r:middle\t" << rm << endl;
    cout << "r:right\t\t" << rr << endl;
    cout << "trapzium\t" << t << endl;
    cout << "simpson\t\t" << s << endl;
    return 0;
}