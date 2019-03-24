#include <iostream>
#include <Eigen/Dense>


using namespace Eigen;
using std::cout;
using std::endl;

int main()
{
    Matrix2d a;
    a << 1, 2,
         3, 4;
    Vector3d v(1, 2, 3);
    std::cout << "a * 2.5 =\n" << a * 2.5 << std::endl;
    std::cout << "0.1 * v =\n" << 0.1 * v << std::endl;
    std::cout << "Doing v *= 2;" << std::endl;
    v *= 2;
    std::cout << "Now v =\n" << v << std::endl;

    cout << "==========================" << endl;
    // MatrixXd b = MatrixXd::Random(3, 3);
    Matrix3d b = Matrix3d::Random();
    cout << "b is = \n" << b << endl;
    cout << "b^T \n" << b.transpose() << endl;
    cout << "conjugate of b is = \n " << b.conjugate() << endl;
    cout << "b^* is \n" << b.adjoint() << endl;

    cout << "==========================" << endl;
    Matrix2d mat;
    mat <<  1, 2,
            3, 4;
    Vector2d u1(-1,1), v1(2,0);
    std::cout << "Here is mat*mat:\n" << mat*mat << std::endl;
    std::cout << "Here is mat*u:\n" << mat*u1 << std::endl;
    std::cout << "Here is u^T*mat:\n" << u1.transpose()*mat << std::endl;
    std::cout << "Here is u^T*v:\n" << u1.transpose()*v1 << std::endl;
    std::cout << "Here is u*v^T:\n" << u1*v1.transpose() << std::endl;
    std::cout << "Let's multiply mat by itself" << std::endl;
    mat = mat*mat;
    std::cout << "Now mat is mat:\n" << mat << std::endl;

    cout << "==========================" << endl;
    Eigen::Matrix2d mat1;
    mat1 << 1, 2,
            3, 4;
    cout << " mat is : \n" << mat1 << endl;
    cout << "Here is mat.sum():       " << mat1.sum() << endl;
    cout << "Here is mat.prod():      " << mat1.prod() << endl;
    cout << "Here is mat.mean():      " << mat1.mean() << endl;
    cout << "Here is mat.minCoeff():  " << mat1.minCoeff() << endl;
    cout << "Here is mat.maxCoeff():  " << mat1.maxCoeff() << endl;
    cout << "Here is mat.trace():     " << mat1.trace() << endl;
}
