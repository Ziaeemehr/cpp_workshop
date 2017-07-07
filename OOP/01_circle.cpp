#include <iostream>
using namespace std;

class Circle
{
    private:
        double radius;
    public:
        void setRadius(double r);
        double getArea();
};

void Circle::setRadius(double r)
{
    radius = r;
}
double Circle::getArea()
{
    return 3.14 * radius * radius;
}

int main()
{
    Circle c1;
    c1.setRadius(2.5);
    cout << c1.getArea()<<'\n';
    return 0;
}
