#include <fstream>
#include <vector>
#include <iostream>
using namespace std;


int main()
{

    std::ifstream infile("file1.txt");


    vector<double> a(10);

    for (int i=0; i<10; i++)
        infile >> a[i];
    infile.close();

    double sum1=0
    for (int i=0; i<10; i++)
        sum1 +=a[i];
    cout <<sum1;
    
    for (int i=0; i<10; i++)
        cout<<a[i]<<"\n";
    
    return 0;
}
