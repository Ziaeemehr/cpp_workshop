#include<iostream>
using namespace std;
class Distance {
private:
    int feet;
    int inches;
public:
    //required constructors
    Distance(){
        feet = 0;
        inches = 0;
    }
    Distance(int f, int i){
        feet = f;
        inches = i;
    }
    void displayDistance(){
        cout << "F : " << feet << " I : " << inches<< endl;
    }
    //overloaded minus (-) operator
    Distance operator- () {
        feet = -feet;
        inches = -inches;
        return Distance(feet,inches);
    }
};

int main(){
    Distance D1(10,20), D2(-17,4);
    -D1;
    D1.displayDistance();
    -D2;
    D2.displayDistance();
    return 0;
}