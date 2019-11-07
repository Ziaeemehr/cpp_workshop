#include <iostream>

using namespace std;

class A
{
    void f();
    void g();
};

void A::f()
{
    // declare pmf as pointer to A member function,
    // taking no args and returning void
    void (A::*pmf)();

    // set pmf to point to A's member function g
    pmf = &A::g;
    // call the member function pointed to by pmf points on this object
    (this->*pmf)(); // calls A::g on this object
}
// using a typedef to preserve sanity - same as above with typedef
// A_pmf_t is a pointer-to-member-function of class A
typedef void (A::*A_pmf_t)();
void A::g()
{
    A_pmf_t p = &A::f;
    (this->*p)(); // calls A::f on this object
}

int main(){
    return 0;
}