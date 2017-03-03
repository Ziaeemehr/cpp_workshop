#include<iostream>

using namespace std;

class Box{
   public:
      double length;
      double breadth;
      double height;
};

int main(){
   Box Box1;
   double volume;

   Box1.height = 5.0;
   Box1.length = 7.0;
   Box1.breadth = 6.0;

   volume = Box1.length*Box1.breadth*Box1.height;
   cout << "Volume of Box1 is: " << volume <<endl;
   return 0;
}
