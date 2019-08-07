#include <iostream>
#include <Eigen/Dense>
using Eigen::MatrixXd;
using std::cout;
using std::endl;

int main()
{
    MatrixXd m(2, 2);
    m(0, 0) = 3;
    m(1, 0) = 2.5;
    m(0, 1) = -1;
    m(1, 1) = m(1, 0) + m(0, 1);
    std::cout << m << std::endl;
    
/*   int rows=10, cols=10;
    double value=0;
    MatrixXd M = MatrixXd::Constant(rows, cols, value).
    std::cout<< MatrixXd::Zero(10)<< std::endl;
*/
    int size=10;
    MatrixXd m1 = MatrixXd::Zero(size, size);
    cout << m1 << endl;
    
    MatrixXd m2;
    m2 = m;
    cout <<m2 << endl;
  
}
