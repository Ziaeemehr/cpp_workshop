#include<iostream>
#include<string>
using namespace std;
class printData {
public:
    void print(int i) {
        cout <<"Printing int: " << i << endl;
    }
    void print(double f) {
        cout <<"Printing float: " << f << endl;
    }
    void print(string c) {
        cout << "Printing character:" << c <<endl;
    }
};

int main(void) {
    printData pd;
    
    pd.print(5);
    pd.print(500.2);
    pd.print("Hello C++");
    
    return 0;
}