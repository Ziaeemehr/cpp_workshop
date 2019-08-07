#include "lib.hpp"

int main()
{
    vector<int> v = {1, 5, 2, 7, 9, 3, 5};
    cout << "average = " << average<int>(v)<< endl;
    cout << "std_dev = " << std_dev<int>(v) << endl;

    return 0;
}

// In [2]: a = [1,5,2,7,9,3,5]
// In [3]: np.mean(a)
// Out[3]: 4.5714285714285712
// In [4]: np.std(a)
// Out[4]: 2.6108095546424379
