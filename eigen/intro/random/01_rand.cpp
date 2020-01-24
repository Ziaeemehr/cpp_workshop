#include <iostream>
#include <Eigen/Dense>
using Eigen::MatrixXd;
using std::cout;
using std::endl;

int main()
{
    MatrixXd m = MatrixXd::Random(3,3).array() + 1.0;
    m /= 2.0;

    cout << a << endl;
}
