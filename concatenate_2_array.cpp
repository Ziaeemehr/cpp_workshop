#include<iostream>
#include <vector>
#include <string>
using namespace std;

int main(){
    
    int arr1[] = {1,2,3,4,5};
    int arr2[] = {6,7,8,9,10};
    int size1=5;
    int size2=5;
    int * result = new int[size1 + size2];
    copy(arr1, arr1 + size1, result);
    copy(arr2, arr2 + size2, result + size1);
    
    for (int i=0; i<10; i++){
        cout << result[i]<<endl;
    }
    
    double arr11[] = {1.,2.,3.,4.,5.};
    double arr21[] = {6.,7.,8.,9.,10.};
    int size11=5;
    int size21=5;
    double * result1 = new double[size11 + size21];
    copy(arr11, arr11 + size11, result1);
    copy(arr21, arr21 + size21, result1 + size11);
    
    for (int i=0; i<10; i++){
        cout << result1[i]<<endl;
    }
    /*-----------------------------------------------------*/
    vector<int> vector1 {1,2,3,4,5};
    vector<int> vector2 {6,7,8,9,10};
    int n = vector1.size()+vector2.size();
    vector<int> conc_v(n);
    vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
    conc_v = vector1;
    cout << " vector concatenation " <<endl;
    for (int i=0; i<vector1.size(); i++)
        cout << conc_v[i]<<endl;
        


    return 0;

}