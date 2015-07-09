#include "yocto/utest/run.hpp"
#include "yocto/gfx/types.hpp"

using namespace yocto;
using namespace gfx;

YOCTO_UNIT_TEST_IMPL(greyscale)
{
    for(int r=0;r<256;++r)
    {
        for(int g=0;g<256;++g)
        {
            for(int b=0;b<256;++b)
            {
                const float f = greyscale<float>(r,g,b);
                if(f<0.0)
                {
                    std::cerr << r << "," << g << "," << b << ": negative!" << std::endl;
                }
                if(f>1.0)
                {
                    std::cerr << r << "," << g << "," << b << ": too big!" << std::endl;
                }
            }
        }
    }
}
YOCTO_UNIT_TEST_DONE()
