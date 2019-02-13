#include <iostream>
using namespace std;

// This makes enum a good choice to work with flags.


enum week
{
    Saturday=1,
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday
};

int main()
{
    week today;
    today = Wednesday;
    cout << "Day " << today << "\n";
    return 0;
}