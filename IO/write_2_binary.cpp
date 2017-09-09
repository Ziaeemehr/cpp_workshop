#include <iostream>
#include <fstream>
#include <string>
using namespace std;
//read from a txt file and write into a binary file

void file_to_binary(string file_name)
{
    ifstream file_to_open(file_name.c_str());
    ofstream file_to_write("output_binary.bin",ios::binary);
    float value;

    if(file_to_open.is_open())
    {
        while(!file_to_open.eof())
        {
            file_to_open >> value;
            file_to_write.write((char*)&value, sizeof(float));
        }
    }
}

int main()
{
    file_to_binary("file1.txt");

    return 0;

}