// This program reads data from a file into an array.

#include <iostream>
#include <fstream> // To use ifstream
#include <vector>
#include <stdlib.h> /* exit, EXIT_FAILURE */

using namespace std;
template<typename T>
std::vector<T> read_from_file(std::string filename)
{
    std::vector<T> numbers;
    std::ifstream inputFile(filename);
    if (inputFile.good())
    {
        T current_number = 0;
        while (inputFile >> current_number)
            numbers.push_back(current_number);

        inputFile.close();
    }

    else
    {
        cout << "Error input file! \n";
        exit(EXIT_FAILURE);
    }
    return numbers;
}

int main()
{
    auto v_int = read_from_file<int>("data/int.txt");
    for (auto d : v_int)
        cout << d << " ";
    cout << endl;

    auto v_double = read_from_file<double>("data/double.txt");
    for (auto d : v_double)
        cout << d << " ";
    cout << endl;

    return 0;
}