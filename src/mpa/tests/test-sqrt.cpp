#include "yocto/utest/run.hpp"
#include "yocto/mpa/rational.hpp"

using namespace yocto;

template <typename T>
T NewtSQRT( const T &x, const T &y )
{
    const T two(2);

    return ((x*x+y)/x)/two;
}

template <typename T>
T SQRT(const T &y, const size_t maxIter )
{
    T x = y;
    for(size_t i=0;i<maxIter;++i)
    {
        std::cerr << "\txold=" << x << std::endl;
        const T xnew = NewtSQRT(x,y);
        std::cerr << "\txnew=" << xnew << std::endl;
        if(xnew==x) return x;
        x=xnew;
    }
    return x;
}

YOCTO_UNIT_TEST_IMPL(sqrt)
{
    const size_t maxIter = 5;
    for(int i=1;i<argc;++i)
    {
        const int64_t val = atoi(argv[i]);
        {
            const int y = val;
            std::cerr << "sqrt(" << y << ") = " << SQRT(y,maxIter) << std::endl;
        }

        {
            const mpn y = val;
            std::cerr << "sqrt(" << y << ") = " << SQRT(y,maxIter) << std::endl;

        }

        {
            const mpz y = val;
            std::cerr << "sqrt(" << y << ") = " << SQRT(y,maxIter) << std::endl;
        }

        {
            const mpq y = val;
            std::cerr << "sqrt(" << y << ") = " << SQRT(y,maxIter) << std::endl;
        }

    }
    
}
YOCTO_UNIT_TEST_DONE()
