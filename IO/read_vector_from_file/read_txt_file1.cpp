// This program reads data from a file into an array.

#include <iostream>
#include <fstream> // To use ifstream
#include <vector>
#include <stdlib.h>     /* exit, EXIT_FAILURE */

using namespace std;

std::vector<int> read_from_file(std::string filename)
{
    std::vector<int> numbers;
    std::ifstream inputFile(filename);
    if (inputFile.good()) 
    {
        int current_number = 0;
        while (inputFile >> current_number)
            numbers.push_back(current_number);

        inputFile.close();
    }
    
    else 
    {
        cout << "Error input file! \n";
        exit (EXIT_FAILURE);
    }
    return numbers;
}


int main()
{
    std::vector<int> numbers;
    ifstream inputFile("data/c.txt");        // Input file stream object

    // Check if exists and then open the file.
    if (inputFile.good()) {
        // Push items into a vector
        int current_number = 0;
        while (inputFile >> current_number){
            numbers.push_back(current_number);
        }

        // Close the file.
        inputFile.close();

        // Display the numbers read:
        cout << "The numbers are: ";
        for (int count = 0; count < numbers.size(); count++){
            cout << numbers[count] << " ";
        }

        cout << endl;
    }else {
        cout << "Error input file! \n";
        exit (EXIT_FAILURE);
    }
    
    auto D = read_from_file("data/c.txt");
    for (auto d : D)
        cout << d << "\n";

    return 0;
}