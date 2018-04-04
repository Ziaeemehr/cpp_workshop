// to_string example
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string

int main ()
{
  std::string pi = "pi is " + std::to_string(3.1415926);
  std::string perfect = std::to_string(25) + " is a perfect number";
  std::cout << pi << '\n';
  std::cout << perfect << '\n';
  return 0;
}
