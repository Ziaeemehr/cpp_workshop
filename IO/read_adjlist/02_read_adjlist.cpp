#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>  

using std::vector;
using std::cout;
using std::endl;
using std::string;

using dim2I = std::vector<std::vector<int> >;

void read_adjlist(
    dim2I& adjlist, 
    const std::string filename, 
    const int n) {
        
    adjlist.resize(n);
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        int counter = 0;

        while (getline(file, line)) {
            std::istringstream iss(line);
            int value;
            while(iss >> value) 
                adjlist[counter].push_back(value);
            counter ++;
        }
    }
    file.close();
}

int main() {

    std::vector<std::vector<int>> a;
    
    std::string filename = "a.adjlist";
    read_adjlist(a, filename, 10);

    for (int i=0; i<a.size(); i++) {
        for (int j=0; j<a[i].size(); j++) {
            cout << a[i][j] << " ";
        }
        cout << "\n";
    }
}

        
