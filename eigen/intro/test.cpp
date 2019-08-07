#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using std::cout;
using std::endl;

int main()
{
    int N= 5;
    VectorXi v(N);
    for (int i=0; i<N; i++)
        v(i) = v(i)+1;
    cout << v << endl;
    
    MatrixXi M = v.asDiagonal();
    
    cout << M << endl;
    
    

}

