#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    vector<string> names;
    names.push_back("John");
    names.push_back("Mary");
    names.push_back("Sue");

    for (int i = 0; i < names.size(); i++)
    {
        cout << names[i] << endl;
    }

    return 0;
}
 
