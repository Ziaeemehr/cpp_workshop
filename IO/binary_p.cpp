#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

template<class T>
char* as_bytes(T&i){
	void* addr = &i;
	return static_cast<char*>(addr);
}

int main()
{
    cout << "enter input filename\n";
    string iname;
    cin >> iname;
    ifstream ifs {iname};
    if (!ifs) perror("can't open input file");

    cout << "enter output filename\n";
    string oname;
    cin>> oname;
    ofstream ofs {oname,ios_base::binary};
    if (!ofs) perror("can't open output file ");

    vector<int> v;
    for (int x; ifs.read(as_bytes(x),sizeof(int));)
        v.push_back(x);

    for (auto x : v)
        cout << x << "\n";
    
    for (int x:v)
        ofs.write(as_bytes(x),sizeof(int));
    return 0;
}