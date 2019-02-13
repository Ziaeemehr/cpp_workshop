#include <iostream>
#include <string>

using namespace std;


class Dog {
    int age;
    string name;
public:
    Dog() {age=3; name="dummy";}
    
    // const parameter
    void setAge(const int& a) { age = a; }
    
    // const return value 
    const string& getName() { return name; }
    
    // const function (does not change any member variable)
    // also only can call other const functions
    void printDogName() const { cout << getName() << " const " << endl;
        // cout << getName() << endl; //Error compile
    }
    
    
    
};

int main()
{
    Dog d;
    const string& n = d.getName();
    cout << n << endl;
}