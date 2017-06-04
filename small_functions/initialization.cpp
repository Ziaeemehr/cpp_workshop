// chap 6 c++ programing language

#include<iostream>
#include<vector>
#include<complex>
#include<cmath>

using std::vector;
using std::string;
using std::cout;
using std::endl;


template<class T> void f1(vector<T>& arg)
{
    for (auto p=arg.begin(); p!=arg.end(); ++p)
    *p = 7;
    // auto instead of vector<T>::iterator
}

int main()
{
    //s simple initialization
    int x4 {};          // 0
    double d4 {};       // 0.0
    char* p {};         // nullptr
    vector<int> v4{};   // empty vector
    string s4 {};       // ""

    auto z2 = 9;        // prefer = when using auto

    // initializer lists p:162
    int a[] = {1,2};
    struct S {int x; string s;};
    S s = {1, "Hello"};
    cout << s.x<< "\t" << s.s<<endl;

    std::complex<double> z = {0, M_PI}; 
    cout << z << endl;

    std::complex<double> z0 {1,2};
    std::complex<double> f2 {} ; //{0,0}

    // use auto in small scopes, unless there is a good reason not to
    
    vector<int> IntA(10);
    f1(IntA);
    for(auto v:IntA)
        printf("%d \n",v);

    return 0;
}
