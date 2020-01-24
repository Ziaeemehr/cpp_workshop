#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <complex>
#include <iostream>

int main()
{
    std::complex<double> a(3.0, 4.0);
    Eigen::MatrixXcd m(2, 2);
    m << a, a,
        0, a;

    // std::cout << m << "\n";

    // Eigen::SparseMatrix<std::complex<double>, Eigen::RowMajor> mat3(4, 4);
    int rows = 4, cols = 4;
    std::vector<int> aij{1, 5, 4, 6};
    std::vector<int> indexi{0, 2, 3, 1};
    std::vector<int> indexj{1, 0, 2, 1};

    Eigen::SparseMatrix<std::complex<int>, Eigen::RowMajor> mat(rows, cols);
    mat.reserve(Eigen::RowVectorXi::Constant(rows, 3));
    for (int i = 0; i < 4; i++)
    {
        mat.insert(indexi[i], indexj[i]) = aij[i];
    }

    Eigen::Matrix<int, rows, cols, Eigen::RowMajor> matrix;

    std::cout << 1 * mat << std::endl;
}