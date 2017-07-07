#include<iostream>
using namespace std;

class Box{
public:
    static int objecCount;
    Box(double l = 2.0, double b=2.0, double h=2.0){
        cout << "constructor called." << endl;
        length = l;
        breadth = b;
        height = h;
        //Increase every time object is created
        objecCount++;
    }
    double Volume(){
        return length*breadth*height;
    }
    static int getCount() {
        return objecCount;
    }
    
private:
    double length;
    double breadth;
    double height;
};
int Box::objecCount = 0;

int main(void){
    cout<< "Initial Stage Count: "<< Box::getCount()<<endl;
    Box Box1(3.3, 1.2, 1.5);    // Declare box1
    Box Box2(8.5, 6.0, 2.0);    // Declare box2
    //print total number of objects after creating object.
    cout << "Final Stage Count: " << Box::getCount()<<endl;
  
    return 0;
}