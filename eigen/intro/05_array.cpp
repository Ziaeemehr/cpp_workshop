#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using std::cout;
using std::endl;

int main()
{
    ArrayXXf a(3,3);
    ArrayXXf b(3,3);

    a << 1,2,3,
         4,5,6,
         7,8,9;
    b << 1,2,3,
        1,2,3,
        1,2,3;

    cout << "a + b \n " << a + b << endl;
    cout << "a - 2 \n"  << a - 2 << endl; 

    // converting array and matrix
    cout << "========================================" << endl;
    MatrixXf m(2,2);
    MatrixXf n(2,2);
    MatrixXf result(2,2);
    m <<  1,2,
          3,4;
    n <<  5,6,
          7,8;
    result = m * n;
    cout << "-- Matrix m*n: --" << endl << result << endl << endl;
    result = m.array() * n.array();
    cout << "-- Array m*n: --" << endl << result << endl << endl;
    result = m.cwiseProduct(n);
    cout << "-- With cwiseProduct: --" << endl << result << endl << endl;
    result = m.array() + 4;
    cout << "-- Array m + 4: --" << endl << result << endl << endl;

    result = (m.array() + 4).matrix() * m;
    cout << "-- Combination 1: --" << endl << result << endl << endl;
    result = (m.array() * n.array()).matrix() * m;
    cout << "-- Combination 2: --" << endl << result << endl << endl;

    return 0;
}