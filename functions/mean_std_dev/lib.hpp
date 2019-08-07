#include <iostream>
#include <vector>
// #include <algorithm>
// #include <functional>
#include <numeric>
#include <assert.h>
#include <cmath>

using std::cout;
using std::vector;
using std::endl;

template <class T>
double average(const vector<T> &v, const int idx=0)
{ /*average the vector from element "id" to end of the vector */
    assert(v.size()>idx);
    double result = accumulate(v.begin() + idx, v.end(), 0.0) / (v.size() - idx);
    return result;
}

template <class T>
double std_dev(const vector<T> &v, const int idx=0)
{
    double ave = average<T>(v, idx);
    double sum_sq = 0.0;
    for (unsigned i=0; i< v.size(); i++)
    {
        double d = v[i]-ave;
        sum_sq += (d*d);
    }
    double result = sqrt(sum_sq/v.size());
    return result;
}


