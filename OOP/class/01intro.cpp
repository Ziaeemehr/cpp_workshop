#include<iostream>

using namespace std;

class Box{
   public:
      double length;
      double breadth;
      double height;
      
      double getVolume(void);
      void setLength(double len);
      void setBreadth(double bre);
      void setHeight(double hei);
};

double Box::getVolume(void){
   return length*breadth*height;
}
void Box::setLength(double len){
   length = len;
}
void Box::setBreadth(double bre){
   breadth = bre;
}
void Box::setHeight(double hei){
   height= hei;
}

int main(){
   Box Box1;
   double volume;

   Box1.setHeight(5.0);
   Box1.setLength(7.0);
   Box1.setBreadth(6.0);

   volume = Box1.getVolume();
   cout << "Volume of Box1 is: " << volume <<endl;
   return 0;
}
