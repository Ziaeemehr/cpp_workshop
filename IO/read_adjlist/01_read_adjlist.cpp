#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using std::vector;
using std::cout;
using std::endl;
using std::string;

typedef std::vector<int> dim1I;
typedef std::vector<dim1I> dim2I;


dim2I read_adjlist(const std::string filename) {
    
    /*read adjacency list from file*/

    dim2I nll;
    std::string str;

    std::ifstream inputFile(filename);
    if (inputFile.good())
    {
        while(std::getline(inputFile, str)) {
            std::istringstream ss(str);
            dim1I nl;
            int i;

            while (ss >> i) {
                nl.push_back(i);
            }
            nll.push_back(nl);
        }
        inputFile.close();
    }

    else
    {
        printf("%s file not found! \n", filename.c_str());
        exit(EXIT_FAILURE);
    }
    return nll;
}


int main() {

    dim2I adjlist;
    std::string filename = "a.adjlist";

    adjlist = read_adjlist(filename);

    for (int i=0; i<adjlist.size(); i++) {
        for (int j=0; j<adjlist[i].size(); j++)
            cout << adjlist[i][j] << " ";
        cout << "\n";
    }
}


std::ifstream file(FILENAME);
if (file.is_open()) {
    std::string line;
    while (getline(file, line)) {
        // using printf() in all tests for consistency
        printf("%s", line.c_str());
    }
    file.close();
}