#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

// https://thispointer.com/c-how-to-find-an-element-in-vector-and-get-its-index/
/*
Generic function to find an element in vector and also its position.
It returns a pair of bool & int i.e.
bool : Represents if element is present in vector or not.
int : Represents the index of element in vector if its found else -1
*/
template <typename T>
std::pair<bool, int> find_in_vector(const std::vector<T> &V, const T &element)
{
    std::pair<bool, int> result;
    // Find given element in vector
    auto it = std::find(V.begin(), V.end(), element);
    if (it != V.end())
    {
        result.second = distance(V.begin(), it);
        result.first = true;
    }
    else
    {
        result.first = false;
        result.second = -1;
    }
    return result;
}

int main()
{
    std::vector<int> vec = {12, 45, 54, 33, 2, 7, 8, 22, 43, 19};
    
    std::pair<bool, int> result = find_in_vector<int>(vec, 450);
    if (result.first)
        std::cout << "Element Found at index : " << result.second <<std::endl;
    else
        std::cout << "Element Not Found" << std::endl;

    return 0;
}