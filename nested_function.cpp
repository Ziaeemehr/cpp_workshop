#include <iostream>

using namespace std;
// int f( int i )
//   {
//     class g_
//     {
//     public:
//       int j;
// 
//       int operator()( int k )
//       {
//         return j+k;
//       }
//     } g;
// 
//     g.j = i*2;
//     g.j += 4;
// 
//     return g( 3 );
//   }     
int main()
{
    auto square = [&](int x) { 
        return x*x; 
    };
    auto a = square(3);
    cout<< a<<endl;

    cout << square(3)<<endl;
    
    auto square1 = [&](int x,int y) { 
        return x*y; 
    };
    auto a1 = square1(3,5);
    cout<< a1 << endl;

    return 0;
}

