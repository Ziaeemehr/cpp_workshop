#include <sstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <string>
#include <sys/stat.h>
#include <iomanip>

using namespace std;

bool folderExists(const std::string& path);
bool fileExists(const std::string &filename);
int main()
{
    
    std::string path = "../data/bin/";
    if (!folderExists(path)) //create folder if does not exist
        auto info = system(("mkdir -p " + path).c_str());

    std::ofstream oX((path + "/X.bin").c_str(), std::ios::out | std::ios::binary);
    std::string ofname = "../data/bin/Y.txt";

    FILE *ofile;
    ofile = fopen(ofname.c_str(), "w");
    if (!fileExists(ofname)){
        cout << "LET file did not open correctly \n!";
        exit(EXIT_FAILURE);
    }

    if(!oX ) {  
	    cout << "Could not open file for output!!";
	    exit(EXIT_FAILURE);
	}


    int N = 2000;

    
    vector<vector<double>> x(N, vector<double>(N)); 
    
    // initialization

    for (int i=0; i<N; i++) 
        for (int j=0; j<N; j++) 
            x[i][j] = (double)rand() / RAND_MAX;

    // writing to file
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++) 
            oX.write((char *)&x[i][j], sizeof(double));


    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            fprintf(ofile, "%18.15f", x[i][j]);
        }
        fprintf(ofile, "\n");
    }
            
    oX.close();
    fclose(ofile);
    
    return 0;
}


//------------------------------------------------------------//
bool folderExists(const std::string& path)
{
    struct stat st;
    if(stat(path.c_str(), &st) == 0) 
    {
        if (st.st_mode & S_IFDIR != 0)
            return true;
    }
    else 
    {
        return false;
    }
}

bool fileExists(const std::string &filename)
{
    /**return true if input file exists*/
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}