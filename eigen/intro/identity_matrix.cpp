#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

using std::cout;
using std::endl;

int main()
{
    int n = 3;
    int N = 9;
    MatrixXd M = MatrixXd::Zero(N, N);
    MatrixXd M00 = MatrixXd::Zero(n, n);
    MatrixXd M01 = MatrixXd::Identity(n, n);
    
    VectorXd v(3);
    VectorXd v1(3);
    for (int i=0; i<n; i++)
    {
        v(i) = -(i+1);
        v1(i) = -1.0;
        
    }
        
    MatrixXd M10 = MatrixXd(v.asDiagonal());
    MatrixXd M02 = MatrixXd(v1.asDiagonal());

    M <<M00, M01, M00,
        M10, M00, M02,
        M00, M00, M00;

    cout << M <<endl;


    

}
