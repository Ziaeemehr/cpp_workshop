#include <iostream>
using namespace std;
class Circle 
{
    private:
        double radius;
    public:
        static int count;
        Circle(double r=1.0)
        {
            radius = r;
            count++; 
        }
        double getArea()
        {
            return 4.131592 *radius*radius;
        }
};

// Initialize static member of class Circle
int Circle::count = 0;

int main()
{
    Circle c1(3.3);
    Circle c2(4.5);
    // Print total number of objects.
    cout << "Total objects: " << Circle::count << endl;

    return 0;
}