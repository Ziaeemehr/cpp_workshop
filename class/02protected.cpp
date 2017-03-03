#include<iostream>

using namespace std;

class Box{
   protected:
      double width;
};

class SmallBox:Box {
//SmallBox is a derived class.
   public:
      double getSmallWidth(void);
      void setSmallWidth(double wid);
};

double SmallBox::getSmallWidth(void){
   return width;
}
void SmallBox::setSmallWidth(double wid){
   width = wid;
}

int main(){
   SmallBox box;
   
   box.setSmallWidth(5.0);
   cout << "Width of box : " << box.getSmallWidth() <<endl;
   
   return 0;
}
