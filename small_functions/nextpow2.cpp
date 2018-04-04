#include <iostream>
#include <math.h>

inline int
pow2roundup (int x)
{
    if (x < 0)
        return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}

int main(int argc, char const *argv[])
{
	int a = 1000*1024;
	auto result = pow2roundup(a);
	std::cout << result <<" which is 2 ^ " << log2(result) << "\n";

	return 0;
}


