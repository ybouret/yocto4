#include "yocto/utest/run.hpp"
#include "yocto/mpa/natural.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(mul)
{
    for(size_t i=0;i<2000;++i)
    {
        const uint64_t l64 = _rand.full<uint32_t>();
        const uint64_t r64 = _rand.full<uint32_t>();
        const uint64_t p64 = l64 * r64;
        
        const mpn l = l64;
        const mpn r = r64;
        const mpn p = l*r;
        if( p.to<uint64_t>() != p64 )
            throw exception("mul error");
        
        const uint64_t sq64 = l64 * l64;
        const mpn      sq   = mpn::sqr(l);
        if( sq.to<uint64_t>() != sq64 )
            throw exception("sqr error");
    }
    
    mpn x = 1;
    std::cerr << x << " => " << mpn::sqr(x) << std::endl;
    x = x * 3;
    std::cerr << x << " => " << mpn::sqr(x) << std::endl;
    x = 2 * x;
    std::cerr << x << " => " << mpn::sqr(x) << std::endl;
    x *= 4;
    std::cerr << x << " => " << mpn::sqr(x) << std::endl;
    x *= x;
    std::cerr << x << " => " << mpn::sqr(x) << std::endl;

    
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(pow)
{
    for(mpn n=2;n<=8;++n)
    {
        mpn n2 = n*(n-1);
        mpn m = mpn::power(n2,n);
        std::cerr << n2 << "^" << n << " = " << m << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()

#include "yocto/sequence/vector.hpp"
#include "yocto/sys/wtime.hpp"

YOCTO_UNIT_TEST_IMPL(mul_perf)
{
    const size_t N =  256;
    vector<mpn> X(N,as_capacity);
    vector<mpn> Y(N,as_capacity);
    wtime       chrono;
    const size_t ITER = 32;
    const size_t Bmax = 16;
    volatile mpn z;
    double sum = 0;
    for(size_t i=1;i<=Bmax;++i)
    {
        double tot = 0;
        for(size_t j=1;j<=Bmax;++j)
        {
            X.free();
            Y.free();
            for(size_t k=0;k<N;++k)
            {
                const mpn x = mpn::rand(i*8);
                const mpn y = mpn::rand(j*8);
                X.push_back(x);
                Y.push_back(y);
            }
            chrono.start();
            for(size_t iter=0;iter<ITER;++iter)
            {
                for(size_t k=N;k>0;--k)
                {
                    (void) mpn::mul(X[k],Y[k]);
                }
            }
            const double ell = chrono.query();
            tot += ell;
        }
        std::cerr << "time" << i << " = " << tot * 1000.0 << " ms" << std::endl;
        sum += tot;
    }
    sum /= ITER;
    std::cerr << "sum=" << sum * 1000.0 << std::endl;
}
YOCTO_UNIT_TEST_DONE()


