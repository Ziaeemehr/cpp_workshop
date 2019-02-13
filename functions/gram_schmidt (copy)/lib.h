#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <assert.h>
#include <iterator>

using namespace std;

using dim1 = std::vector<double>;
using dim2 = std::vector<std::vector<double>>;

namespace std
{
template <typename T>
vector<T> operator+(const vector<T> &a, const vector<T> &b)
{
    assert(a.size() == b.size());
    vector<T> result;
    result.reserve(a.size());
    transform(a.begin(), a.end(), b.begin(),
              back_inserter(result), plus<T>());
    return result;
}
} // namespace std
/*------------------------------------------------------------*/
inline double dot(const dim1& a, const dim1& b) {
    return std::inner_product(std::begin(a), std::end(a), std::begin(b), 0.0);
}
/*------------------------------------------------------------*/
inline double norm(const dim1& a) {
    return sqrt(dot(a,a));
}
/*------------------------------------------------------------*/
dim1 proj(const dim1& a, const dim1& b) {
    assert (a.size()==b.size());
    
    double coeff = dot(a,b);
    
    int n = a.size();
    dim1 res(n);
    
    for (int i=0; i<n; i++)
        res[i] = b[i] * coeff;
    
    return res;
}
