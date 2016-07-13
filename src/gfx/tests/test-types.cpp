#include "yocto/gfx/color/yuv.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/string.hpp"
#include <cmath>

using namespace yocto;
using namespace gfx;


YOCTO_UNIT_TEST_IMPL(types)
{
    for(int r=0;r<256;++r)
    {
        for(int g=0;g<256;++g)
        {
            for(int b=0;b<256;++b)
            {
                const float gs = gist::greyscalef(r,g,b);
                if(gs<0.0f)
                {
                    throw exception("negative greyscale!");
                }
                if(gs>1.0f)
                {
                    throw exception("excessive greyscale!");
                }
            }
        }
    }

    RGB  C(1,2,3); std::cerr << "C=" << C << std::endl;
    RGBA D(4,5,6); std::cerr << "D=" << D << std::endl;
    std::cerr << "D'=" << D._rgb() << std::endl;

    std::cerr << "sizeof(RGB) =" << sizeof(RGB)  << std::endl;
    std::cerr << "sizeof(RGBA)=" << sizeof(RGBA) << std::endl;
    std::cerr << "sizeof(YUV) =" << sizeof(YUV)  << std::endl;



}
YOCTO_UNIT_TEST_DONE()
