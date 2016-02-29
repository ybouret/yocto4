#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"

#include "yocto/ios/bitio.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(io)
{
    for(size_t i=0;i<10;++i)
    {
        const uint64_t x = _rand.full<uint64_t>();
        const mpn      n = x;
        const mpn      m = n;
        std::cerr << std::hex;
        std::cerr << "x=" << x << std::endl;
        std::cerr << "n=" << n << std::endl;
        std::cerr << "m=" << m << std::endl;
        const uint64_t y = m.to<uint64_t>();
        std::cerr << "y=" << y << std::endl;
        std::cerr << std::endl;
        
        if(x!=y) throw exception("invalid conversion");
    }
    
    ios::bitio bio;
    for(size_t i=0;i<10;++i)
    {
        bio.free();
        const mpn x = _rand.full<uint64_t>();
        x.store(bio);
        if(bio.size()!=x.bits()) throw exception("bio store failure");
        const mpn y = mpn::query(bio, bio.size());
        if( x != y ) throw exception("bio query failure");
    }
    
    mpn X;
    for(size_t i=0; i < 10; ++i )
    {
        std::cerr << std::hex << "0x" << X << " = " << std::dec << X << std::endl;
        X = _rand.full<uint64_t>();
    }
    
}
YOCTO_UNIT_TEST_DONE()


#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/sequence/vector.hpp"
#define INPROC line.clear(),\
fprintf( stderr, "> " ),\
fflush(stderr),\
inp.read_line(line) >= 0

YOCTO_UNIT_TEST_IMPL(parse)
{
    vector<mpn> u;
    {
        ios::ocstream fp( "mpn.dat", false);
        ios::icstream inp( ios::cstdin );
        string line;
        while(INPROC)
        {
            const mpn x = mpn::parse( line );
            //std::cerr << std::dec << x << " / 0x" << std::hex << x << std::endl; std::cerr.flush();
            x.save(fp);
            u.push_back(x);
        }
    }
    
    {
        ios::icstream fp("mpn.dat");
        for(size_t i=1;i<=u.size();++i)
        {
            const mpn x = mpn::load(fp);
            if( x != u[i] )
                throw exception("I/O error");
        }
    }
    
}
YOCTO_UNIT_TEST_DONE()
