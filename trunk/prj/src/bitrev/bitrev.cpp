#include "yocto/ios/ocstream.hpp"
#include <iostream>
#include "yocto/code/utils.hpp"


using namespace yocto;


int main(int argc, char *argv[] )
{
    
    try
    {
        
        for( size_t p=0; p < 12; ++p )
        {
            const size_t size = 1 << p;
            std::cerr << "size=" << size << std::endl;
            size_t nops = 0;
            const size_t n    = size << 1;
            {
                size_t j=1;
                for (size_t i=1; i<n; i+=2) {
                    if (j > i)
					{
                        ++nops;
                        //std::cerr << '(' << i << ',' << j << ')' << ' ';
                    }
                    size_t m = size; // m=  n / 2;
                    while (m >= 2 && j > m)
					{
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }
            }
            std::cerr << "\tnops = " << nops << " *2";
            std::cerr << std::endl;
        }
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "Error" << std::endl;
    }
    return 1;
}
