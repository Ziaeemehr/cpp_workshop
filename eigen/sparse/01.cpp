#include <Eigen/Sparse>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

typedef Eigen::Triplet<int> T;
typedef Eigen::Triplet<double> Td;

using namespace Eigen;

int main(int argc, char **argv)
{
    SparseMatrix<double> mat1(1000, 2000);              // declares a 1000x2000 column-major compressed sparse matrix of double
    SparseMatrix<double, RowMajor> mat2(1000, 2000);    // declares a 1000x2000 row-major compressed sparse matrix of double
    SparseMatrix<std::complex<float>> mat3(1000, 2000); // declares a 1000x2000 column-major compressed sparse matrix of complex<float>

    SparseVector<std::complex<float>> vec0(1000); // declares a column sparse vector of complex<float> of size 1000
    SparseVector<double, RowMajor> vec1(1000);    // declares a row sparse vector of double of size 1000

    // cout << mat1.rows() << endl;
    // cout << mat1.cols() << endl;
    // cout << mat1.nonZeros() << endl;

    cout << "============================================================" << endl;

    // Filling a sparse matrix -----------------------------------------//
    int rows = 4, cols = 4;
    vector<int> aij{1, 5, 4, 6};
    vector<int> indexi{0, 2, 3, 1};
    vector<int> indexj{1, 0, 2, 1};
    vector<double> aijd{1., 5.0, 4.0, 6.0};

    // method 1
    {
        SparseMatrix<int, RowMajor> mat(rows, cols);
        mat.reserve(Eigen::RowVectorXi::Constant(rows, 3));
        // method 1
        for (int i = 0; i < 4; i++)
        {
            mat.insert(indexi[i], indexj[i]) = aij[i];
        }
    }

    // method 2
    {
        vector<T>
            tripletList;
        SparseMatrix<int, RowMajor> mat(rows, cols);
        tripletList.reserve(4);
        for (size_t i = 0; i < 4; i++)
            tripletList.push_back(T(indexi[i], indexj[i], aij[i]));

        mat.setFromTriplets(tripletList.begin(), tripletList.end());
    }

    // method 2 double
    {
        vector<Td>
            tripletList;
        SparseMatrix<double> mat(rows, cols);
        tripletList.reserve(4);
        for (size_t i = 0; i < 4; i++)
            tripletList.push_back(Td(indexi[i], indexj[i], aijd[i]));

        mat.setFromTriplets(tripletList.begin(), tripletList.end());

        cout << mat << endl;
    }
    // Basic operations ------------------------------------------------//
    // cout << 2 * mat << endl;

    // SparseMatrix<int, RowMajor> A;
    // A = mat;
    // cout << mat + A << endl;

    // MatrixXi B(4, 4);
    // B << 1, 1, 1, 1,
    //     2, 2, 2, 2,
    //     1, 1, 1, 1,
    //     2, 2, 2, 2;
    // cout << mat << endl;
    // cout << mat * B << endl;

    // Sparse and Dense matrix conversion ------------------------------//
    {
        SparseMatrix<double> spMat;
        MatrixXd dMat;

        // Sparse to dense:
        dMat = MatrixXd(spMat);

        // Dense to sparse:
        spMat = dMat.sparseView();
    }
    return 0;
}