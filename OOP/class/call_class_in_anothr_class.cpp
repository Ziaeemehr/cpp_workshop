// Using an instance of a class inside another class [stack]

#include <iostream>

class First
{
private:
    int n;

public:
    First(int n) : n(n){};

    int add(int a, int b)
    {
        return a + b + n;
    }
};

class Second
{
private:
    First objF;

public:
    Second(int n) : objF(n){};
    void display(int a, int b)
    {
        std::cout << objF.add(a, b) << std::endl;
    }
};

int main()
{
    Second obj(1);
    obj.display(5, 10);

    return 0;
}
