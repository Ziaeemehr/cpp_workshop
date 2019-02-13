#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	int N=10;
    for (int ens = 0; ens<2; ens++)
    {
        vector<double> vec1;
        for (int i=0; i<N; i++)
            vec1.push_back(i*(ens+1.0));

        for (int i=0; i<N; i++)
            cout << vec1[i]<< " ";
        cout << endl;
        cout <<vec1.size() << endl;


    }

	
	return 0;
}