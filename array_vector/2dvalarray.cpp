#include <iostream>
#include <valarray>

using namespace std;

typedef valarray<valarray<int> > va2d;

int main()
{
    int data[][3] = { {1, 2, 3}, {4, 5, 6} };
    va2d mat(valarray<int>(2), 3);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
           mat[ i ][ j ] = data[ i ][ j ];
    }
    for (int i = 0; i < 2; ++i){
        for (int j = 0; j < 3; ++j)
           cout << mat[ i ][ j ] << " ";
        cout<< endl;
    }
    return 0;
}
