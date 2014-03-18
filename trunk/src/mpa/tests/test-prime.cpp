#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;


#define TABLE_SIZE (4096)

static inline void out( ios::ostream &fp, const mpn &p, size_t &count )
{
    for(size_t i=0;i<p.length();++i)
    {
        fp("0x%02x, ",p.get_byte(i));
        if( 0 == (++count%16) ) fp("\n");
    }
}

YOCTO_UNIT_TEST_IMPL(prime)
{
    
    mpn    n     = mpn::three();
    vector<mpn>  prm;
    prm.push_back(n);
    
    while(true)
    {
        while( ! (++n).is_prime16() )
            ;
        mpn d = n - prm.back();
        assert( d.is_even() );
        d >>= 1;
        
        if(d.bits()>8)
            break;
        std::cerr << "d=" << d << std::endl;
        prm.push_back(n);
        
        if(prm.size()>=TABLE_SIZE)
            break;
    }
    std::cerr << "#prm=" << prm.size() << std::endl;
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(prime24)
{
#if 0
    for(size_t i=0;i<10;++i)
    {
        const mpn n = _rand.full<uint64_t>();
        std::cerr << n;
        std::cerr.flush();
        if(n.is_prime24())
        {
            std::cerr << " YES" << std::endl;
        }
        else
        {
            std::cerr << " NO" << std::endl;
        }
        }
#endif
        }
        YOCTO_UNIT_TEST_DONE()
        
