// http://www.cplusplus.com/reference/utility/pair/pair/
// The difference is that with std::pair you need to specify the types of both
// elements, whereas std::make_pair will create a pair with the type of the 
// elements that are passed to it, without you needing to tell it.

#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>     // std::cout

int main () {
  std::pair <std::string,double> product1;                     // default constructor
  std::pair <std::string,double> product2 ("tomatoes",2.30);   // value init
  std::pair <std::string,double> product3 (product2);          // copy constructor

//   product1 = std::make_pair(std::string("lightbulbs"),0.99);   // using make_pair (move)
  product1 = std::make_pair("lightbulbs",0.99);   // using make_pair (move)
  
  
  product2.first = "shoes";                  // the type of first is string
  product2.second = 39.90;                   // the type of second is double

  std::cout << "The price of " << product1.first << " is $" << product1.second << '\n';
  std::cout << "The price of " << product2.first << " is $" << product2.second << '\n';
  std::cout << "The price of " << product3.first << " is $" << product3.second << '\n';
  return 0;
}