#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(bits)
{
    std::cerr << std::hex;
    for(size_t i=0;i<=100;++i)
    {
        mpn x = mpn::exp2(i);
        std::cerr << "2^" << i << " = 0x" << x << std::endl;
        while( !x.is_zero() )
        {
            std::cerr << "\t" << x.shr() << std::endl;
        }
    }
    
    for(size_t i=0;i<100;++i)
    {
        uint64_t x64 = _rand.full<uint64_t>();
        mpn      x   = x64;
        std::cerr << "x=" << x << ", 0x" << x.bits() << " bits" << std::endl;
        for(size_t j=alea_leq(12);j>0;--j)
        {
            const size_t n = alea_leq(12);
            x64 >>= n;
            x   >>= n;
            std::cerr << "\t" << x << " / " << x64 << std::endl;
            if( x.to<uint64_t>() != x64 )
                throw exception("shr failure");
        }
    }
    
    for(size_t i=0;i<100;++i)
    {
        mpn x = alea_leq(500);
        for(size_t j=alea_leq(12);j>0;--j)
        {
            const size_t n = alea_leq(12);
            const mpn    y = x;
            std::cerr << x << " << 0x" << n << " = ";
            x <<= n;
            std::cerr << x << std::endl;
            const mpn    z = mpn::shr(x,n);
            if( y != z)
                throw exception("shl failure");
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
