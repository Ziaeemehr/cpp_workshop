#include <Eigen/Sparse>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

using namespace Eigen;
typedef Eigen::SparseMatrix<int, Eigen::RowMajor> SRI;

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

    cout << 1 * mat << endl;

    mat.coeffRef(0, 1) = 0;

    cout << 1 * mat << endl;
    
    cout << mat.cols() << mat.rows() << endl;
    
    for (int i=0; i<mat.rows(); i++)
    {
        for (int j=0; j<mat.cols(); j++)
        {
            cout << mat.coeffRef(i, j) << " ";
        }
        cout << endl;
    }

    return 0;
}
