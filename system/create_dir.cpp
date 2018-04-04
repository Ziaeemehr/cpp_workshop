#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


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



bool fileExists(const std::string& filename)
{
    /*return true if input file exists*/
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

int main(int argc, char const *argv[])
{
	if (!folderExists("example"))
		printf("%s\n", "no" );
	else
		printf("%s\n", "yes" );



/*
	struct stat st;
	if(stat("example",&st) == 0) 
	{
	    if (st.st_mode & S_IFDIR != 0)
	        printf(" example is present\n");
	}
	else 
	{
		printf(" example is not present\n");
	}
*/



	// bool isdir = S_ISDIR(st.st_mode);
	


	// if(foldefExists("example"))
	// {
	// 	system("mkdir example");
	// 	printf("%s\n", "folder created!");
	// }
	// else
	// {
	// 	printf("%s\n", "folder exists!");
	// }
	return 0;
}