#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
using namespace std;

int main()
{
    using namespace boost::lambda;
    typedef istream_iterator<int> in;
    
    for_each(
	in(cin), in(), cout << (_1 * 3) << " " );
    cout << endl;
} 
