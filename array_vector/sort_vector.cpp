#include <vector>
#include <iostream>
#include <algorithm>
using namespace std; 
  
int main() 
{ 
    vector<int> v{ 1, 5, 8, 9, 6, 7, 3, 4, 2, 0 }; 
  
    sort(v.begin(), v.end()); 
  
    cout << "Sorted \n"; 
    for (auto x : v) 
        cout << x << " "; 
    cout << "\n";
  
    return 0; 
} 