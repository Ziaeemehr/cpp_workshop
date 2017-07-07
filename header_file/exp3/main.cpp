#include <iostream>
using namespace std;
#include "Time.h"

int main()
{
    Time t1(10,50,59);
    t1.print();
    Time t2;
    t2.print();
    t2.setTime(6,39,9);
    t2.print();

    if(t1.equals(t2))
          cout << "Two objects are equal\n";
     else
          cout << "Two objects are not equal\n";	
  
     return 0;
}