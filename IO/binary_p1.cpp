#include<iostream>
#include<fstream>
#include<cstdio>
using namespace std;
class Student
{
    int admno;
    char name[50];
public:
    void setData()
    {
        cout << "\nEnter admission no. ";
        cin >> admno;
        cout << "Enter name of student ";
        cin.getline(name,50);
    }

    void showData()
    {
        cout << "\nAdmission no. : " << admno;
        cout << "\nStudent Name : " << name;
    }
	 
    int retAdmno()
    {
        return admno;
    }
};


/*
* function to write in a binary file.
*/
void write_record()
{
    ofstream outFile;
    outFile.open("student.dat", ios::binary | ios::app);

    Student obj;
    obj.setData();
    
    outFile.write((char*)&obj, sizeof(obj));
    
    outFile.close();
}

/*
* function to display records of file
*/

void display()
{
    ifstream inFile;
    inFile.open("student.dat", ios::binary);

    Student obj;
    
    while(inFile.read((char*)&obj, sizeof(obj)))
    {
        obj.showData();
    }        
    
    inFile.close();
}