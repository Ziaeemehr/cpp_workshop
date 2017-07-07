//One class memmber function can access the private and protected 
//members of other class. We do it by declaring a class as friend
//of other class.

#include <iostream>
using namespace std;
class CostCalculator;

class Rectangle
{
    private:
        int length;
        int width;
    public:
        void setData(int len, int wid)
        {
            length = len;
            width = wid;
        }
        int getArea()
        {
            return length*width;
        }
        friend class CostCalculator; //friend of class Rectangle
};

//friend class costCalculator can access private member of class Rectangle
class CostCalculator
{
    public:
        double getCost (Rectangle rect)
        {
            double cost;
            cost = rect.length * rect.width * 5; 
            return cost;
        }
};

int main ()
{
    Rectangle floor;
    floor.setData(20,3);
    CostCalculator calc;
    cout << "Expense " << calc.getCost(floor) << endl;
    return 0;
}