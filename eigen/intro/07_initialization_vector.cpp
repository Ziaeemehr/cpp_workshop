#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <valarray>

using namespace Eigen;
using namespace std;

// using std::cout;
// using std::endl;
// using std::vector;

int main()
{
//     vector<double> v1 = {1.0, 2.0, 3.0};
//     Eigen::Vector3d v2;
//     v2 << v1[0] , v1[1], v1[2];


//     vector<double> u1 = {1.0, 2.0, 3.0};
//     Eigen::Vector3d u2(u1.data());
//     cout << u2 << endl;
    
//     vector<double> w1(4,10.0);
//     double * ptr = &w1[0];
//     Eigen::Map<Eigen::VectorXd> w2(ptr, 4);
//     cout << w2 << endl;
    
    std::vector<double> a = {1, 2, 3, 4};
    Eigen::VectorXd b = Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(a.data(), a.size());
    cout << b << endl;
    cout << b(0) << endl;
    
    Eigen::Vector3d a1(1.0, 2.0, 3.0);
    Eigen::Vector3d a2(1.5, 2.5, 3.5);
    
    cout << a1+a2 << endl;
       
    vector<double> vec(a2.data(), a2.data() + a2.size());
    valarray<double> val(vec.data(), vec.size());
    
    for (auto i : val)
        cout << i << " ";
    cout << endl;
    
    
    
    
    return 0;

}

