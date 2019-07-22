#include <cstdio>

class c1
{
    int i = 0;

public:
    void setvalue(const int &value) { i = value; }
    int getvalue() const;
    int getvalue2() const;
};

int c1::getvalue() const 
{
    printf("getvalue: this is %p\n", this);
    return i;
}
int c1::getvalue2() const 
{
    printf("getvalue2 \n");
    return this->getvalue();
}

int main()
{
    const int i = 47;
    c1 o1;
    o1.setvalue(i);
    printf("address of o1 is %p\n", &o1);
    printf("value is %d\n", o1.getvalue2());
    return 0;
}