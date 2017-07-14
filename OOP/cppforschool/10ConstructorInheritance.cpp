#include<iostream>
using namespace std;

class Rectangle
{
private :
	float length;
	float width;
public:
	Rectangle ()
	{
		length = 0;
		width = 0;
	}
    Rectangle (float len, float wid)
	{ 
		length = len;
		width = wid; 
	}
    float area()
	{
		return length * width ;
	}
};

class Box : public Rectangle
{
    private:
        float height;
    public: 
        Box()
        {
            height = 0;
        }
        Box (float len, float wid, float ht) : Rectangle(len,wid)
        {
            height = ht;
        }
        float volume()
        {
            return area() * height;
        }
};

int main()
{
    Box bx;
    Box cx(4,8,5);
    cout << bx.volume() << endl;
    cout << cx.volume() << endl;
    return 0;
}