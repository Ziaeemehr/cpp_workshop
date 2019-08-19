#include <Eigen/Sparse>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

using namespace Eigen;
typedef Eigen::SparseMatrix<int, Eigen::RowMajor> SRI;

// ------------------------------------------------------------//

template <class T>
T sparseMax(SparseMatrix<T> &mat)
{
    return Map<Matrix<T, Dynamic, 1>>(mat.valuePtr(), mat.nonZeros()).maxCoeff();
}
// ------------------------------------------------------------//

int maxCoeff(const SparseMatrix<int, RowMajor> &A)
{
    size_t row = A.rows();
    size_t col = A.cols();
    int max_value = -10000000;
    for (size_t k = 0; k < row; k++)
    {
        for (SRI::InnerIterator it(A, k); it; ++it)
        {
            if (it.value() > max_value)
                max_value = it.value();
        }
    }
    return max_value;
}

// ------------------------------------------------------------//

int main()
{
    // Filling a sparse matrix
    int rows = 4, cols = 4;
    vector<int> aij{1, 5, 4, 6};
    vector<int> indexi{0, 2, 3, 1};
    vector<int> indexj{1, 0, 2, 1};

    SparseMatrix<int, RowMajor> mat(rows, cols);
    for (int i = 0; i < 4; i++)
        mat.insert(indexi[i], indexj[i]) = aij[i];

    cout << mat << endl;

    // method1
    cout << maxCoeff(mat) << endl;

    // method2
    if (!mat.isCompressed())
        mat.makeCompressed();
    auto max = mat.coeffs().maxCoeff();
    cout << max << endl;

    // method3
    if (!mat.isCompressed())
        mat.makeCompressed();
    auto max = sparseMax(mat);

    return 0;
}