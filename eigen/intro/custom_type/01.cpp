#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

typedef Matrix< unsigned long int, Dynamic, Dynamic> MatrixXULI;
typedef Matrix< int, Dynamic, Dynamic> MatrixXI;


void f1(long unsigned int a){
    cout << a << endl;
}
    

int main()
{
    MatrixXULI m(2,2);
    MatrixXI m1(2,2);

    m(0,0) = 100000000000;
    m1(0,0) = 100;

    cout << m << endl;
    cout << m1 << endl;
    
    f1(m1(0,0));
    
    return 0;
}
