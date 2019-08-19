#include <Eigen/Sparse>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

typedef Eigen::Triplet<int> T;

using namespace Eigen;

int main(int argc, char **argv)
{
    SparseMatrix<std::complex<float>> mat0(1000, 2000); // declares a 1000x2000 column-major compressed sparse matrix of complex<float>
    SparseMatrix<double, RowMajor> mat1(1000, 2000);    // declares a 1000x2000 row-major compressed sparse matrix of double
    SparseVector<std::complex<float>> vec0(1000);       // declares a column sparse vector of complex<float> of size 1000
    SparseVector<double, RowMajor> vec1(1000);          // declares a row sparse vector of double of size 1000

    cout << mat1.rows() << endl;
    cout << mat1.cols() << endl;
    cout << mat1.nonZeros() << endl;

    // Filling a sparse matrix
    int rows = 4, cols = 4;
    vector<int> aij{1, 5, 4, 6};
    vector<int> indexi{0, 2, 3, 1};
    vector<int> indexj{1, 0, 2, 1};

    vector<T> tripletList;
    tripletList.reserve(4);

    for (size_t i = 0; i < 4; i++)
        tripletList.push_back(T(indexi[i], indexj[i], aij[i]));

    SparseMatrix<int, RowMajor> mat(rows, cols);
    mat.setFromTriplets(tripletList.begin(), tripletList.end());
    // for (int i=0; i<4; i++)
    // {
    //     mat.insert(indexi[i], indexj[i]) = aij[i];
    // }

    cout << 2 * mat << endl;

    exit(0);
    // Basic operations
    SparseMatrix<int, RowMajor> A;
    A = mat;
    cout << mat + A << endl;

    MatrixXi B(4, 4);
    B << 1, 1, 1, 1,
        2, 2, 2, 2,
        1, 1, 1, 1,
        2, 2, 2, 2;
    cout << mat << endl;
    cout << mat * B << endl;

    return 0;
}