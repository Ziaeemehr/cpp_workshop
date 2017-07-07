#include <iostream>
using namespace std;

class Circle
{
    private:
        double radius;
    public:
        Circle()
        { 
            radius = 0;
        }
        Circle(double r)
        {
            radius =r;
        }
        Circle(Circle &t)
        {
            radius = t.radius;
        }
        void setRadius(double r);
        double getArea();
};

void Circle::setRadius(double r)
{
    radius = r;
}
double Circle::getArea()
{
    return 3.141592 * radius * radius;
}

int main()
{
    Circle c1;
    Circle c2(2.5);
    Circle c3(c2);
    cout << c1.getArea()<<'\n';
    cout << c2.getArea()<<'\n';
    cout << c3.getArea()<<'\n';
    return 0;
}

// Another way of Member initialization in constructors
// Circle :: Circle(double r)
// {
//     radius = r;
// }
// Circle :: Circle(double r) : radius(r)
// {}
