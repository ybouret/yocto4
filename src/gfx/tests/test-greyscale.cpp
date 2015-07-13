#include "yocto/utest/run.hpp"
#include "yocto/gfx/types.hpp"

using namespace yocto;
using namespace gfx;

namespace
{
    template <typename T,typename FUNC>
    static inline void test_conv(FUNC         &func,
                                 const uint8_t r,
                                 const uint8_t g,
                                 const uint8_t b,
                                 type2type<T>,
                                 const char   *fn
                                 )
    {
        const T f = func(r,g,b);
        if(f<T(0))
        {
            std::cerr << fn << "@ " << r << "," << g << "," << b << ": negative greyscale!" << std::endl;
        }
        if(f>T(1))
        {
            std::cerr << fn << "@ " << r << "," << g << "," << b << ": too big greyscale!" << std::endl;
        }
    }


}

#define __TEST_CONV(T,F) test_conv(F,r,g,b,type2type<T>(),#F)

YOCTO_UNIT_TEST_IMPL(greyscale)
{
    for(int r=0;r<256;++r)
    {
        for(int g=0;g<256;++g)
        {
            for(int b=0;b<256;++b)
            {
                __TEST_CONV(float,greyscale<float>);
                __TEST_CONV(float,black_and_white<float>);

            }
        }
    }
}
YOCTO_UNIT_TEST_DONE()
