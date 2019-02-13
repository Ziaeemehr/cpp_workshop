#include <vector>
#include <iostream>

using namespace std;

using dim1 = std::vector<double>;
using dim2 = std::vector<std::vector<double>>;

dim2 f1(const int n)
{
    dim1 v1(n);
    dim1 v2(2*n);
    for (int j=0; j<n; j++)
            v1[j] = 0.1;
    for (int j = 0; j < (2*n); j++)
        v2[j] = 0.2;

    dim2 V = {v1, v2};
    return V;

}

int main(int argc, char const *argv[])
{
    auto V = f1(10);
    dim1 v1 = V[0];
    dim1 v2 = V[1];
    for (int i=0; i<v2.size(); i++)
        cout << v2[i]<< " ";
    cout << "\n";

    return 0;
}
