#include <iostream>
#include <algorithm> // for all_of()

using namespace std;

int main()
{
    // Initializing array
    double ar[6] = {1.0, 2.0, 3.0, 4.0, 5.0, -6.0};

    // Checking if all elements are positive
    if(all_of(ar, ar + 6, [](double x) { return x > 0; }))
        cout << "All are positive elements\n";
    else
        cout << "All are not positive elements\n";

    return 0;
}