#include <Eigen/Sparse>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

using namespace Eigen;

int main(int argc, char **argv)
{

    // Filling a sparse matrix
    int rows = 4, cols = 4;
    vector<int> aij{1, 5, 4, 6};
    vector<int> indexi{0, 2, 3, 1};
    vector<int> indexj{1, 0, 2, 1};

    //-----------------------------------------------------------------------
    Eigen::SparseMatrix<int, Eigen::RowMajor> mat_row(5, 4);
    mat_row.reserve(Eigen::RowVectorXi::Constant(5, 3));
    for (int i = 0; i < 4; i++)
        mat_row.insert(indexi[i], indexj[i]) = aij[i];

    // cout << mat_row.outerSize() << endl;
    cout << 1 * mat_row << endl;
    // loop over nonzero elements (row major)
    cout << "value, \t row, \t col" << endl;
    
    for (int k = 0; k < mat_row.outerSize(); ++k)
        for (SparseMatrix<int, Eigen::RowMajor>::InnerIterator it(mat_row, k); it; ++it)
        {
            cout << it.value() << "\t"
                 << it.row() << "\t"    // row index
                 << it.col() << endl;    // col index (here it is equal to k)
        }
    
    mat_row.coeffRef(1, 1) = 0;
    mat_row.coeffRef(1, 2) = 10;
    mat_row.prune(0);
    cout << 1*mat_row << endl;
    
    for (int k = 0; k < mat_row.outerSize(); ++k)
        for (SparseMatrix<int, Eigen::RowMajor>::InnerIterator it(mat_row, k); it; ++it)
        {
            cout << it.value() << "\t"
                 << it.row() << "\t"    // row index
                 << it.col() << endl;    // col index (here it is equal to k)
        }
    

    return 0;
}
