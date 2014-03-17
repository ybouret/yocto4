#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(bits)
{
    for(size_t i=0;i<=100;++i)
    {
        mpn x = mpn::exp2(i);
        std::cerr << "2^" << i << " = 0x" << x << std::endl;
        while( !x.is_zero() )
        {
            std::cerr << "\t" << x.shr() << std::endl;
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
