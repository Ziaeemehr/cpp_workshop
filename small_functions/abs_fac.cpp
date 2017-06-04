#include<iostream>
template<class T> T abs(T a) {return a<0 ? -a : a;}
constexpr int fac(int n) {return (n<2) ? 1 : n*fac(n-1);}

int main()
{
    int n {5};
    
    std::cout<< abs(n) << std::endl;
    std::cout<< abs(-1*n) << std::endl;
    std::cout<< fac(n) << std::endl;

    constexpr double zz{fac(7)};
    std::cout << zz <<std::endl;
    
    return 0;
}