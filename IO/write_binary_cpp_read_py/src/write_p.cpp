#include <sstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <string>
#include <sys/stat.h>

using namespace std;

bool folderExists(const std::string &path);

int main()
{

    std::string path = "../data/bin/";
    if (!folderExists(path)) //create folder if does not exist
        auto info = system(("mkdir -p " + path).c_str());

    std::ofstream oX((path + "/X.bin").c_str(), std::ios::out | std::ios::binary);
    std::ofstream oY((path + "/Y.bin").c_str(), std::ios::out | std::ios::binary);
    std::ofstream oW((path + "/W.bin").c_str(), std::ios::out | std::ios::binary);
    std::ofstream oZ((path + "/Z.bin").c_str(), std::ios::out | std::ios::binary);

    if (!oX || !oY || !oZ || !oW)
    {
        cout << "Could not open file for output!!";
        exit(EXIT_FAILURE);
    }

    int N = 10;

    std::vector<double> x(N);
    vector<vector<double>> y(N, vector<double>(3));
    vector<vector<float>> w(N, vector<float>(3));
    vector<vector<int>> z(N, vector<int>(3));
    // initialization

    for (int i = 0; i < N; i++)
    {
        x[i] = i + 0.0;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            y[i][j] = i + j;
            w[i][j] = i + j;
            z[i][j] = i + j;
        }
    }

    // writing to file
    oX.write((char *)&x[0], x.size() * sizeof x[0]);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            oY.write((char *)&y[i][j], sizeof(double));
            oW.write((char *)&w[i][j], sizeof(float));
            oZ.write((char *)&z[i][j], sizeof(int));
        }
    }

    oX.close();
    oY.close();
    oZ.close();
    oW.close();

    return 0;
}

//------------------------------------------------------------//
bool folderExists(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
    {
        if (st.st_mode & S_IFDIR != 0)
            return true;
    }
    else
    {
        return false;
    }
}
