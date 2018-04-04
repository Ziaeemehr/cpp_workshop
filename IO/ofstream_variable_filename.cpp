#include <fstream>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
	string filename = "example1"+to_string(1);
	ofstream fout(filename.c_str(), ios::out | ios::trunc);
	// ofstream fout(filename.c_str());
	fout << "Writing this to a file.\n";
	fout.close();


	double D=3.2;
	string filename1 = "example1"+to_string((int)D)+".txt";
	ofstream fout1(filename1.c_str());
	fout1 << "Writing this to a file.\n";
	fout1.close();
	return 0;
}

