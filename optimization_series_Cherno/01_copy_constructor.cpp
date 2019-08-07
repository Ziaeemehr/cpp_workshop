#include <iostream>
#include <string> 

using std::cout;
using std::endl;

struct Vector2
{
    float x, y;
};

int main()
{
    Vector2 l = {2,3};
    Vector2 m = l;
    l.x = 5;
    cout << l.x << " " << m.x << endl;
    

    Vector2* a = new Vector2();
    a->x = 2;
    a->y = 10;
    Vector2* b = a;

    b->x = 3;

    cout <<  a->x << " " << b->x << endl;


    return 0;
}