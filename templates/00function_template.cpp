#include <iostream>
#include <string>

using namespace std;

template <typename T>
inline T const& Max (T const& a, T const& b) {
    return a < b ? b:a;
}

template <class T>
inline T GetMax(T a, T b) {
    return (a>b?a:b);
}

template <class T, class U>
inline T GetMin(T a, U b) {
    return (a<b?a:b);
}

int main () {
 
   int i = 39;
   int j = 20;
   cout << "Max(i, j): " << Max(i, j) << endl; 
   cout << "Max(i, j): " << GetMax(i, j) << endl; 
   
   
   double f1 = 13.5; 
   double f2 = 20.7; 
   cout << "Max(f1, f2): " << Max(f1, f2) << endl; 
   cout << "Max(f1, f2): " << GetMax(f1, f2) << endl; 

   string s1 = "Hello"; 
   string s2 = "World"; 
   cout << "Max(s1, s2): " << Max(s1, s2) << endl; 
   cout << "Max(s1, s2): " << GetMax(s1, s2) << endl; 

   cout << "Min(i, f2): " << GetMin(i, f2) << endl;  

   return 0;
}