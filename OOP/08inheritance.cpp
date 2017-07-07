#include<iostream>
using namespace std;
// class derived_class: memberAccessSpecifier base_class
// { 
// 	... 
// };

class Shape
{
    protected:
    // private:
        float width, height;
    public:
        void set_data(float a, float b)
        {
            width  = a;
            height = b;
        }
        // friend class Rectangle;
        // friend class Triangle;
};
// if i define the width and height as private, I have to define the Rectangle and 
// Triangle as friend class to have access to them
class Rectangle: public Shape 
{
    public:
        float area()
        {
            return (width*height);
        }
};

class Triangle: public Shape 
{
    public:
    float area()
    {
        return (width*height/2.0);
    }
};

int main()
{
    Rectangle rect;
    Triangle tri;
    rect.set_data(5,3);
    tri.set_data(2,5);
    cout << rect.area() <<endl;
    cout << tri.area()  <<endl;
    return 0;
}