#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>

inline bool file_exist(const std::string &name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

bool fileExists(const std::string &filename)
{
    /*return true if input file exists*/
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

int main(int argc, const char **argv)
{
    std::cout << file_exist("test_file.txt") << std::endl;
    std::cout << fileExists("test_file.txt") << std::endl;
    return 0;
}