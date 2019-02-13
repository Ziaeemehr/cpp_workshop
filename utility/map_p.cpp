// constructing maps
#include <iostream>
#include <map>
#include <vector>
#include <string>

bool fncomp (char lhs, char rhs) {return lhs<rhs;}

struct classcomp {
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};

template<typename TKeys, typename TValues>
void print_map(std::map<TKeys,TValues> &m)
{
    // Iterating over the map
    for (auto entry : m) {
        std::cout << entry.first << " :: ";
		for (auto i : entry.second)
			std::cout << i << " , ";
		std::cout << std::endl;
    }
    
}

int main ()
{
  std::map<char,int> first;

  first['a']=10;
  first['b']=30;
  first['c']=50;
  first['d']=70;

    std::map<char,int> second (first.begin(),first.end());

    std::map<char,int> third (second);

    std::map<char,int,classcomp> fourth;                 // class as Compare

    bool(*fn_pt)(char,char) = fncomp;
    std::map<char,int,bool(*)(char,char)> fifth (fn_pt); // function pointer as Compare

    /*----------------------------------------------------------*/
    
    std::map<int, std::string> userNameForUserID;
    // insert a user
    userNameForUserID[10] = std::string("John Doe");
    std::cout << "User #10 is named: " << userNameForUserID[10] << std::endl;

    /*----------------------------------------------------------*/
    
    // Initialize a Map of string & vector of int using initializer_list
    std::map<std::string, std::vector<int> > mapOfOccur = 	{
                            { "Riti", { 3, 4, 5, 6 } },
                            { "Jack", { 1, 2, 3, 5 } }
                            };
    print_map(mapOfOccur);    
    
    /*----------------------------------------------------------*/
    
    std::map<std::string, std::vector<double> > stateVec = {
    {"x", {2.1}}};
    print_map(stateVec);
    
    /*----------------------------------------------------------*/
    
    std::map<std::string, std::vector<int>> stateVec1;
    for (int i=0; i<10; i++)
    {
        stateVec1["x"].push_back(i);
    }    
    print_map(stateVec1);

    /*----------------------------------------------------------*/
    std::map <std::string, std::vector<std::string> > dict;
    std::string usa[] = { "NYC" , "LA" , "Chicago" , "Dallas" };
    dict["USA"] = std::vector<std::string>(usa,usa+4);
    print_map(dict);
    
  
  return 0;
}