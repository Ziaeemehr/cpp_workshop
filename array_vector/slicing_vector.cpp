#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	std::vector<double> myVec(20);
	for (int i=0; i<20; i++)
		myVec[i] = i ;

	// vector<double>::const_iterator first = myVec.begin() + 10;
	// vector<double>::const_iterator last = myVec.end();
	// vector<double> newVec(first, last);

	std::vector<double> newVec(myVec.begin () + 10, myVec.end());
	
	for (auto v : newVec)
		cout << v << endl;


	return 0;
}