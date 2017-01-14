#include<iostream> 
#include<vector>
#include<fstream>
#include <string>
using namespace std;


const int n=10;

void func(vector<double>& a,int p){
    for(int i=0; i<p; i++)
        a[i] = i + 0.1 ;
}

int main()
{
    vector<double> b(10);
    for(int i=0; i<n; i++){
	cout<< b[i]<<endl;
    }
    cout << endl;
    func(b,10);
    for(int i=0; i<n; i++){
	cout<< b[i]<<endl;
    }
    return 0;
}
