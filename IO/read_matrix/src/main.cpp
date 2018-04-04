#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>

using namespace std;

typedef std::vector<double> dim1;
typedef std::vector<std::vector<double>> dim2;

bool fileExists(const std::string &filename);
std::vector<std::vector<double>> read_matrix(int Node, std::string filename);
/*------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    int N = 5;
    string filename = "../data/C.txt"; //here give the txt file address
    dim2 A = read_matrix(N, filename);

    for (int i = 0; i < A.size(); i++) //loop over rows
    {
        for (int j = 0; j < A[0].size(); j++) //loop over columns
        {
            cout << A[i][j] << "\t";
        }
        cout << endl;
    }

    return 0;
}

/*------------------------------------------------------------*/
bool fileExists(const std::string &filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}
/*------------------------------------------------------------*/
std::vector<std::vector<double>> read_matrix(int Node, std::string filename)
{
    std::ifstream ifile(filename);

    if (fileExists(filename)) //check if file exists in the given address
    {
        std::vector<std::vector<double>> Cij(Node, dim1(Node));
        for (int i = 0; i < Node; i++)
        {
            for (int j = 0; j < Node; j++)
            {
                ifile >> Cij[i][j];
            }
        }
        ifile.close();
        return Cij;
    }
    else
    {
        std::cerr << "\n file : " << filename << " not found \n";
        exit(2);
    }
}

/*------------------------------------------------------------*/
