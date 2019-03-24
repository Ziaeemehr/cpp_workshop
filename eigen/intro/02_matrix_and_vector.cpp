#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

int main()
{
    MatrixXd m = MatrixXd::Random(3,3);
    m = (m + MatrixXd::Constant(3,3,1.2))*50;
    cout << "m = " << endl << m << endl;
    VectorXd v(3);
    v << 1, 2, 3;
    cout << "m * v = " << endl << m *v << endl;

    Matrix3d m1 = Matrix3d::Random();
    m1 = (m1 + Matrix3d::Constant(1.2)) * 50;
    cout << "m1 =" << endl
         << m1 << endl;
    Vector3d v1(1, 2, 3);

    cout << "m1 * v1 =" << endl
         << m1 * v1 << endl;

    return 0;
}