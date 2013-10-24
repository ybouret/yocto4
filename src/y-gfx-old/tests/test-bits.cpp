#include "yocto/utest/run.hpp"
#include "yocto/gfx/types.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(bits)
{
    for(int i=0; i < 256 ; ++i )
    {
        const uint8_t b = i;
        const string  s = gfx::conv::to_binary(b);
        std::cerr << i << " => " << s << std::endl;
    }
    
    for( int i=0; i <10; ++i )
    {
        const float f = _rand.get<float>();
        std::cerr << f << " ==> " << int(gfx::conv::f2b(f)) << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
