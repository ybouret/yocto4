#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;


#define TABLE_SIZE (16*1024)

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
    
    size_t bytes = 0;
    size_t n1    = 0;
    size_t n2    = 0;
    size_t n3    = 0;
    mpn    n     = mpn::one();
    vector<mpn>  prm;
    
    while(bytes<TABLE_SIZE)
    {
        if( n.is_prime16() )
        {
            std::cerr << n << std::endl;
            prm.push_back(n);
            bytes += n.length();
            switch(n.length())
            {
                case 0:
                    throw exception("Prime Error");
                
                case 1:
                    ++n1;
                    break;
                    
                case 2:
                    ++n2;
                    break;
                    
                case 3:
                    ++n3;
                    break;
                    
                default:
                    throw exception("Too Big!");
            }
        }
        n.inc(2);
    }
    std::cerr << "bytes = " << bytes << std::endl;
    std::cerr << "n1    = " << n1 << std::endl;
    std::cerr << "n2    = " << n2 << std::endl;
    std::cerr << "n3    = " << n3 << std::endl;
    mpn nxt;
    if( bytes >= TABLE_SIZE )
    {
        nxt = prm.back();
        prm.pop_back();
        bytes -= nxt.length();
        switch (nxt.length())
        {
            case 1:
                --n1;
                break;
                
            case 2:
                --n2;
                break;
                
            case 3:
                --n3;
                break;
                
            default:
                break;
        }
    }
    else
    {
        nxt = prm.back();
        while( !nxt.is_prime16() ) nxt.inc(2);
    }
    std::cerr << std::endl;
    std::cerr << "bytes = " << bytes << std::endl;
    std::cerr << "n1    = " << n1 << std::endl;
    std::cerr << "n2    = " << n2 << std::endl;
    std::cerr << "n3    = " << n3 << std::endl;
    std::cerr << "nxt   = " << nxt << std::endl;
    
    ios::ocstream fp("p24.dat",false);
    size_t idx   = 0;
    size_t count = 0;
    for(size_t i=1;i<=n1;++i)
    {
        out(fp,prm[++idx],count);
    }
    
    for(size_t i=1;i<=n2;++i)
    {
        out(fp,prm[++idx],count);
    }
    
    for(size_t i=1;i<=n3;++i)
    {
        out(fp,prm[++idx],count);
    }

    fp("0\n");

}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(prime24)
{
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
}
YOCTO_UNIT_TEST_DONE()

