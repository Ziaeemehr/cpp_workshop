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
    //overloaded < operator
    bool operator <(const Distance& d){
        if (feet < d.feet){
            return true;
        }
        if(feet == d.feet && inches<d.inches){
            return true;
        }
        return false;
    }
};

int main(){
    Distance D1(11,10), D2(5,11);
    if (D1<D2){
        cout << "D1<D2" << endl;
    } else{
        cout << "D2<D1" << endl;
    }
    
    return 0;
}