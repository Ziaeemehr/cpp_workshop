#include <iostream>
#include <fstream>
#include <assert.h>
#include <sys/stat.h>
// Function: fileExists
/*
    Check if a file exists
@param[in] filename - the name of the file to check

@return    true if the file exists, else false

*/
bool fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

int main()
{
    if(fileExists("infile.txt")){
        std::cout<<"yes"<<"\n";
        std::ifstream iFile("input.txt");
        while (!iFile.eof())
        {  
            int x;
            iFile >> x;
            if (!iFile.eof()) break;
            std::cout << x << std::endl;
        }
//         ifile.close();
    }
    else {
        std::cout<<"NO"<<"\n";
    }
    return 0;
        
}
