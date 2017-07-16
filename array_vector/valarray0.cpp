#include <iostream>
#include <valarray>

using namespace std;

int main () {
   std::valarray<int> myvalarray;
   std::cout << "1. After construction: " << myvalarray.size() << '\n';

   myvalarray = std::valarray<int>(15);
   std::cout << "2. After assignment: " << myvalarray.size()<< '\n';
   
   for (int i=0; i<15; i++)
       std::cout <<" "<< myvalarray[i];
   std::cout <<"\n";
   myvalarray.resize(30);
   std::cout << "3. After downsizing: " << myvalarray.size() << '\n';

   myvalarray.resize(100);
   std::cout << "4. After resizing up: " << myvalarray.size() << '\n';

   valarray<int> myarray (100,50);
   cout << myarray.size()<<endl;
   
   return 0;
}
