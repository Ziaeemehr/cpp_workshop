// iota example
#include <iostream>     // std::cout
#include <numeric>      // std::iota
#include <vector>

int main () {
  int numbers[10];
  std::iota (numbers,numbers+10,100);

  std::cout << "numbers:";
  for (int& i:numbers) std::cout << ' ' << i;
  std::cout << '\n';
  
  
  std::vector<int> v(14);
  std::iota(v.begin(), v.end(), 3);
  for (auto i:v)
      std::cout << i << " ";
  std::cout<<"\n";

  return 0;
}