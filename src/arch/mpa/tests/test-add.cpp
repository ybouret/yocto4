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
    const size_t lb = length * 8;
    const size_t nb = lb/2 + alea_leq(lb);
    return mpn::rand(nb);
}

#include <cmath>
#include "yocto/code/utils.hpp"


static inline
void ave_and_rms( double &ave, double &rms, const array<double> &v )
{
    ave = 0;
    rms = 0;
    const size_t n = v.size();
    for(size_t i=n;i>0;--i)
    {
        ave += v[i];
    }
    ave /= n;
    for(size_t i=n;i>0;--i)
    {
        const double d = v[i] - ave;
        rms += d*d;
    }
    rms = sqrt(rms/n);
}

static inline
double gen_sample( vector<mpn> &lhs, vector<mpn> &rhs, size_t nperf, size_t bytes)
{
    double ave = 0;
    lhs.free();
    rhs.free();
    for(size_t i=nperf;i>0;--i)
    {
        const mpn x = generate_mpn(bytes);
        lhs.push_back(x);
        rhs.push_back(x);
        ave += x.bits();
    }
    return ave/nperf;
}

YOCTO_UNIT_TEST_IMPL(add_perf)
{
    volatile mpn sum;
#if 0
    for(size_t nb=1;nb<=32;++nb)
    {
        double ab = 0;
        for(size_t i=0;i<10;++i)
        {
            const mpn x = mpn::rand(nb);
            std::cerr << " " << x;
            ab += x.bits();
        }
        ab /= 10;
        std::cerr << " <" << ab << "/" << nb <<  ">" << std::endl;
    }
    
    return 0;
#endif
    
    wtime        chrono;
    
    const size_t nperf  = 2048;
    const size_t abits  = 1024;
    const size_t cycles = 64;
    vector<mpn>  lhs(nperf,as_capacity);
    vector<mpn>  rhs(nperf,as_capacity);
    
    const size_t bytes = abits/8;
    
    vector<double> spd(cycles,as_capacity);
    
    chrono.start();

    for(size_t k=1;k<=cycles;++k)
    {
        const double ave = gen_sample(lhs, rhs, nperf, bytes);
        double       ell = 0;
        size_t       na  = 0;
        for(size_t i=nperf;i>0;--i)
        {
            const mpn &l = lhs[i];
            const mpn &r = rhs[i];
            double tsub =0;
            const double mark = chrono.query();
            do
            {
                (mpn &)sum = mpn::add(l,r);
                tsub = chrono.query() - mark;
                ++na;
            }
            while(tsub<5e-4);
            
            ell += chrono.query() - mark;
        }
        ell/=na;
        std::cerr << "add=" << na  << std::endl;
        std::cerr << "ave=" << ave << std::endl;
        std::cerr << "ell=" << ell << std::endl;
        
        spd.push_back( 1e-6*ave/ell );
        double speed = 0;
        double rms   = 0;
        ave_and_rms(speed,rms,spd);
        const double err = rms/sqrt(double(k));
        std::cerr << "\tSPEED = " << speed << " Mbits/s, +/- " << err << std::endl;
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
