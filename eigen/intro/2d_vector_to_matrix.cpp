#include <vector>
#include <iostream>
#include <Eigen/Dense>


int main()
{
    using namespace Eigen;
    std::vector<std::vector<double> > data(10, std::vector<double>(4));
    for (int i=0; i<10; i++)
        for (int j=0; j<4; j++)
            data[i][j] = i+j;
        
    
    MatrixXd mat(10, 4);
    for (int i = 0; i < 10; i++)
        mat.row(i) = VectorXd::Map(&data[i][0],data[i].size());
    
    std::cout << mat;
    
    return 0;
}

