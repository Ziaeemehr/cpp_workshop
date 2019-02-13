#include <vector>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

using namespace std; 

int main() 
{ 
    int arr1[] = {1, 2, 3}; 
    int arr2[] = {4, 5, 6}; 
    int n = sizeof(arr1)/sizeof(arr1[0]); 
    int res[n]; 

    // Single line code to add arr1[] and arr2[] and 
    // store result in res[] 
    transform(arr1, arr1+n, arr2, res, plus<int>()); 

    for (int i=0; i<n; i++) 
        cout << res[i] << " "; 
    cout << endl;

    return 0;
}


