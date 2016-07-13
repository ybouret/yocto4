#include "yocto/gfx/types.hpp"
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



}
YOCTO_UNIT_TEST_DONE()
