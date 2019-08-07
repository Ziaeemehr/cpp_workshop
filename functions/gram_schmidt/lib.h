#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <numeric>
#include <assert.h>
#include <iterator>
#include <math.h>

// using namespace std;

using dim1 = std::vector<double>;
using dim2 = std::vector<std::vector<double>>;
using std::cout;
using std::endl;
using std::vector;

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
inline double dot(const dim1 &a, const dim1 &b)
{
    return std::inner_product(std::begin(a), std::end(a), std::begin(b), 0.0);
}
/*------------------------------------------------------------*/
inline double norm(const dim1 &a)
{
    return sqrt(dot(a, a));
}
/*------------------------------------------------------------*/
dim1 proj(const dim1 &a, const dim1 &b)
{
    assert(a.size() == b.size());

    double coeff = dot(a, b);

    int n = a.size();
    dim1 res(n);

    for (int i = 0; i < n; i++)
        res[i] = b[i] * coeff;

    return res;
}
/*------------------------------------------------------------*/
void gram_schmidt(dim1 &A, const int n, dim1 &znorm)
{
    dim1 B(n * n);
    auto first_A = A.begin();
    auto first_B = B.begin();

    // iterator beg1 = first, end1 = first + n;
    std::fill(znorm.begin(), znorm.end(), 0.0);

    dim1 bp(first_A, first_A + n);
    znorm[0] = norm(bp);

    for (int i = 0; i < n; i++)
        B[i] = bp[i] / (znorm[0] + 0.0);

    for (int k = 1; k < n; k++)
    {
        dim1 sumj(n);
        for (int j = 0; j < k; j++)
        {
            dim1 a(first_A + (k * n), first_A + (k + 1) * n);
            dim1 b(first_B + (j * n), first_B + ((j + 1) * n));
            sumj = sumj + proj(a, b);
        }

        for (int i = 0; i < n; i++)
            bp[i] = A[k * n + i] - sumj[i];

        znorm[k] = norm(bp);
        for (int i = 0; i < n; i++)
            B[k * n + i] = bp[i] / znorm[k];
    }

    dim1 a1(n);
    dim1 a2(n);
    dim1 a3(n);
    for (int i = 0; i < n; i++)
    {
        a1[i] = B[i];
        a2[i] = B[n + i];
        a3[i] = B[2 * n + i];
    }

    cout << dot(a1, a2) << endl;
    cout << dot(a1, a3) << endl;
    cout << dot(a2, a3) << endl;

    for (int i = 0; i < n; i++)
        cout << znorm[i] << " ";
    cout << endl;

    A = B;
}
/*------------------------------------------------------------*/

void gram_schmidt_2(dim1 &A, dim1 &znorm)
{
    const int n = 2;
    dim1 B(n * n);
    auto first_A = A.begin();
    auto first_B = B.begin();
    std::fill(znorm.begin(), znorm.end(), 0.0);

    dim1 bp(first_A, first_A + n);
    znorm[0] = norm(bp);

    for (int i = 0; i < n; i++)
        B[i] = bp[i] / (znorm[0] + 0.0);
    
    for (int k = 1; k < 2; k++)
    {
        dim1 sumj(n);
        for (int j = 0; j < k; j++)
        {
            dim1 a(first_A + (k * n), first_A + (k + 1) * n);
            dim1 b(first_B + (j * n), first_B + ((j + 1) * n));
            sumj = sumj + proj(a, b);
        }

        for (int i = 0; i < n; i++)
            bp[i] = A[k * n + i] - sumj[i];

        znorm[k] = norm(bp);
        for (int i = 0; i < n; i++)
            B[k * n + i] = bp[i] / znorm[k];
    }


    dim1 a1(n);
    dim1 a2(n);
    dim1 a3(n);
    for (int i = 0; i < n; i++)
    {
        a1[i] = B[i];
        a2[i] = B[n + i];
        a3[i] = A[2 * n + i];
    }

    cout << dot(a1, a2) << endl;
    cout << dot(a1, a3) << endl;
    cout << dot(a2, a3) << endl;

    for (int i = 0; i < n; i++)
        cout << znorm[i] << " ";
    cout << endl;

    A = B;
}