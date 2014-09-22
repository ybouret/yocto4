#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(add)
{
    for(size_t i=0;i<10;++i)
    {
        const uint64_t l64 = _rand.full<uint32_t>();
        const uint64_t r64 = _rand.full<uint32_t>();
        const uint64_t s64 = l64 + r64;
        
        const mpn l = l64;
        const mpn r = r64;
        const mpn s = mpn::add(l,r);
        
        const uint64_t t64 = s.to<uint64_t>();
        
        if(t64!=s64)
            throw exception("add error");
    }
    
    mpn x; //!< zero
    for(size_t i=1;i<=1000000;++i)
    {
        ++x;
        if( x.to<size_t>() != i )
            throw exception("++ error");
    }
    
    std::cerr << std::dec;
    x  = uint64_t(0);
    std::cerr << x << std::endl;
    x += 2;
    std::cerr << x << std::endl;
    x  = 3 + x;
    std::cerr << x << std::endl;
    x  = x + 5;
    std::cerr << x << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sequence/vector.hpp"
#include "yocto/sys/wtime.hpp"

static inline
mpn generate_mpn(size_t length)
{
    mpn x;
    const size_t m = length/2;
    
    for(size_t i=0;i<m;++i)
    {
        x <<= 8;
        x  += uint8_t(alea_leq(255));
    }
    
    for(size_t i=alea_leq(length);i>0;--i)
    {
        x <<= 8;
        x  += uint8_t(alea_leq(255));
    }
    
    return x;
}

YOCTO_UNIT_TEST_IMPL(add_perf)
{
    volatile mpn sum;
    
    wtime        chrono;
    vector<mpn>  lhs;
    vector<mpn>  rhs;
    const size_t nperf  = 1000;
    const size_t abits  = 512;
    const size_t cycles = 8;
    
    const size_t bytes = abits/8;
    
    vector<double> tmx(cycles,as_capacity);
    vector<double> ws(cycles,as_capacity);
    chrono.start();
    
    for(size_t k=1;k<=cycles;++k)
    {
        double ave = 0;
        for(size_t i=0;i<nperf;++i)
        {
            const mpn l = generate_mpn(bytes);
            lhs.push_back(l);
            ave += l.bits();
            const mpn r = generate_mpn(bytes);
            rhs.push_back(r);
            ave += r.bits();
        }
        ave /= (nperf*2);
        std::cerr << "bits=" << ave << std::endl;
        ws.push_back(ave);
        
        double ell = 0;
        for(size_t i=nperf;i>0;--i)
        {
            const double mark = chrono.query();
            (mpn &)sum = mpn::add(lhs[i],rhs[i]);
            ell += chrono.query() - mark;
        }
        std::cerr << "ell=" << ell << std::endl;
        tmx.push_back(ell);
        std::cerr << "\tspd=" << ws[k]/tmx[k] << std::endl;
    }
    double speed = 0;
    for(size_t k=1;k<=cycles;++k)
    {
        speed += ws[k]/tmx[k];
    }
    speed /= cycles;
    std::cerr << "speed = " << speed << " bits/s" << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
