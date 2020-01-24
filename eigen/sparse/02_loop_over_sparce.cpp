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

    Eigen::SparseMatrix<int> mat(4, 4);
    mat.reserve(Eigen::VectorXi::Constant(4, 3));
    for (int i = 0; i < 4; i++)
        mat.insert(indexi[i], indexj[i]) = aij[i];

    cout << 1*mat << endl;

    // loop over nonzero elements (colom major)
    cout << "value, \t row, \t col, \t index" << endl;
    for (int k = 0; k < mat.outerSize(); ++k)
        for (SparseMatrix<int>::InnerIterator it(mat, k); it; ++it)
        {
            cout << it.value() << "\t"
                 << it.row() << "\t"    // row index
                 << it.col() << "\t"    // col index (here it is equal to k)
                 << it.index() << endl; // inner index, here it is equal to it.row()
        }
    //-----------------------------------------------------------------------
    Eigen::SparseMatrix<int, Eigen::RowMajor> mat_row(5, 4);
    mat.reserve(Eigen::RowVectorXi::Constant(5, 3));
    for (int i = 0; i < 4; i++)
        mat_row.insert(indexi[i], indexj[i]) = aij[i];

    // cout << mat_row.outerSize() << endl;
    // cout << mat_row << endl;
    // loop over nonzero elements (row major)
    cout << "value, \t row, \t col, \t index" << endl;
    
    for (int k = 0; k < mat_row.outerSize(); ++k)
        for (SparseMatrix<int, Eigen::RowMajor>::InnerIterator it(mat_row, k); it; ++it)
        {
            cout << it.value() << "\t"
                 << it.row() << "\t"    // row index
                 << it.col() << "\t"    // col index (here it is equal to k)
                 << it.index() << endl; // inner index, here it is equal to it.row()
        }
    exit(0);
    //-----------------------------------------------------------------------
    // access to an element
    // int value = mat.coeff(iRow, iCol);

    // vector
    cout << "vector: value, index" << endl;
    SparseVector<double> vec(10);
    vec.insert(0) = 1.5;
    vec.insert(3) = 2.0;
    for (SparseVector<double>::InnerIterator it(vec); it; ++it)
    {
        cout << "\t" << it.value() << "\t" // == vec[ it.index() ]
             << it.index() << endl;
    }



    {
        cout << "Row Major double" << endl;
        Eigen::SparseMatrix<double, Eigen::RowMajor> mat(5, 4);
        mat.reserve(Eigen::RowVectorXi::Constant(5, 3));
        for (int i = 0; i < 4; i++)
            mat.insert(indexi[i], indexj[i]) = aij[i];

        // cout << mat << endl;
        // loop over nonzero elements (row major)
        cout << "value, \t row, \t col, \t index" << endl;
        cout << mat_row.outerSize() << endl;
        
        for (int k = 0; k < mat_row.outerSize(); ++k)
            for (SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(mat, k); it; ++it)
            {
                cout << it.value() << "\t"
                    << it.row() << "\t"    // row index
                    << it.col() << "\t"    // col index (here it is equal to k)
                    << it.index() << endl; // inner index, here it is equal to it.row()
            }
    }


    return 0;
}
