#include <iostream>
using namespace std;
/* The static functions can access only the static data
 * of a class. Similarly, static functions cannot call 
 * non-static functions of the class.
 */
class Circle
{
    private:
        static int count;   
        double radius;     // Radius of a circle
    public:
        // Constructor definition
        Circle(double r = 1.0)
        {
            radius = r;
            // Increase every time object is created
            count++;
        }
        double getArea()
        {
            return 3.14 * radius * radius;
        }
        static int getCount()
        {
            return count;
        }
};
// Initialize static member of class Circle
int Circle::count = 0;
int main()
{
    Circle c1(3.3);    // Declare object c1
    Circle c2(4.5);    // Declare object c2
 
    // Print total number of objects.
    cout << "Total objects: " << Circle::getCount() << endl;

    return 0;
}