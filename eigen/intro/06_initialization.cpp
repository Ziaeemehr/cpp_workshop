#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using std::cout;
using std::endl;

int main()
{
    Matrix3f m;
    m << 1, 2, 3,
        4, 5, 6,
        7, 8, 9;
    std::cout << m << "\n";

    RowVectorXd vec1(3);
    vec1 << 1, 2, 3;
    std::cout << "vec1 = " << vec1 << std::endl;
    RowVectorXd vec2(4);
    vec2 << 1, 4, 9, 16;
    std::cout << "vec2 = " << vec2 << std::endl;
    RowVectorXd joined(7);
    joined << vec1, vec2;
    std::cout << "joined = " << joined << std::endl;

    MatrixXf matA(2, 2);
    matA << 1, 2, 3, 4;
    MatrixXf matB(4, 4);
    matB << matA, matA / 10, matA / 10, matA;
    std::cout << matB << std::endl;

    std::cout << "A fixed-size array:\n";
    Array33f a1 = Array33f::Zero();
    std::cout << a1 << "\n\n";
    std::cout << "A one-dimensional dynamic-size array:\n";
    ArrayXf a2 = ArrayXf::Zero(3);
    std::cout << a2 << "\n\n";
    std::cout << "A two-dimensional dynamic-size array:\n";
    ArrayXXf a3 = ArrayXXf::Zero(3, 4);
    std::cout << a3 << "\n";

    cout << "LinSpaced vector \n";
    cout << VectorXi::LinSpaced(4,7,10).transpose() << "\n";
    cout << VectorXd::LinSpaced(5,0.0,1.0).transpose() << endl;
    cout << VectorXi::LinSpaced(8, 1, 7).transpose() << endl;
}

