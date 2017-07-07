//Operator overloading is giving new functionality to an existing operator.
//It means the behavior of operators when applied to objects of a class 
//can be redefined.

#include <iostream>
using namespace std;

class Complex 
{
    private:
        double real;
        double imag;
    public:
        Complex() {};
        Complex(double, double);
        Complex operator + (Complex);
        void print();
};

Complex::Complex(double r, double i)
{
    real = r;
    imag = i;
}

Complex Complex::operator+(Complex param)
{
    Complex temp;
    temp.real = real + param.real;
    temp.imag = imag + param.imag;
    return (temp);
}

void Complex::print()
{
    cout << real << " + i" << imag << endl;
}

int main()
{
    Complex c1 (3.1, 1.5);
    Complex c2 (1.2, 2.2);
    Complex c3;

    c3 = c1 + c2;
    c1.print();
    c2.print();
    c3.print();

    return 0;
}