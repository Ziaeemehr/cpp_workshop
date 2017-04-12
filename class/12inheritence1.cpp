#include<iostream>
using namespace std;

//Base class Sahpe
class Shape {
public:
    void setWidth(int w) {
        width = w;
    }
    void setHeight(int h) {
        height = h;
    }
protected :
    int width;
    int height;
};

// Base class PaintCost
class PaintCost {
public:
    int getCost(int area) {
        return area*70;
    }
};

//Detived class
class Rectangle: public Shape, public PaintCost {
public:
    int getArea() {
        return (width*height);
    }
};

int main(void) {
    Rectangle Rect;
    int area;
    
    Rect.setWidth(5);
    Rect.setHeight(7);
    
    area = Rect.getArea(); 
    
    cout << "Total area : " << Rect.getArea() << endl;
    cout << "Total paint cost: $" << Rect.getCost(area)<<endl;
    return 0;
}