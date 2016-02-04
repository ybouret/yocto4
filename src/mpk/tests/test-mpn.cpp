#include "yocto/utest/run.hpp"
#include "yocto/mpk/word2mpn.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/comparator.hpp"
#include "yocto/random/bits.hpp"

using namespace yocto;
using namespace mpk;

#define _SHOW(X) std::cerr << #X " = " << X << std::endl
#define _SHOW_BITS(X) std::cerr << #X ".bits = " << (X).bits() << std::endl

YOCTO_UNIT_TEST_IMPL(mpn)
{
    // basic
    std::cerr << "-- Basic Tests" << std::endl;
    {
        natural  n0;    _SHOW(n0);
        word2mpn w0(0); _SHOW(w0.n);
        uint64_t u = _rand.full<uint64_t>();
        std::cerr << std::hex; _SHOW(u); std::cerr << std::dec;
        natural  n1(u);  _SHOW(n1);
        word2mpn w1(u);  _SHOW(w1.n);
        n0=n1; _SHOW(n0);
        n0=0;  _SHOW(n0);
        n0=u;  _SHOW(n0);

        for(size_t i=0;i<100;++i)
        {
            word_t x = _rand.full<word_t>();
            n0 = x;
            word_t y = n0.to_word();
            //std::cerr << std::hex << x << " -> " << n0 << " -> " << y << std::dec << std::endl;
            if(x!=y)
            {
                throw exception("invalid to_word...");
            }
        }
    }

    // comparison
    std::cerr << "-- Comparison Tests" << std::endl;
    Random::Bits &ran = Random::CryptoBits();
    {
        for(size_t i=0;i<1000;++i)
        {
            word_t  x = ran.fuzz<word_t>();
            natural n = x;
            word_t  y = ran.fuzz<word_t>();
            natural m = y;
            if( 0 != natural::compare(n,n) )
            {
                throw exception("zero compare failure");
            }
            if( ! natural::are_equal(m,m) )
            {
                throw exception("equality failure");
            }

            const int c = __compare(x,y);
            const int C = natural::compare(n,m);
            if( c != C )
            {
                std::cerr << "failure " << std::hex << x << " vs " << y << std::dec << " -> " << n << " vs " << m << std::endl;
                throw exception("comparison mismatch: %d and %d",c,C);
            }
            if(C!=0)
            {
                if( ! natural::are_different(n,m) )
                    throw exception("difference failure");
            }

        }
    }

    // addition
    {
        for(natural i=0;i<100;++i,i++)
        {
            std::cerr << i << std::endl;
        }

        for(size_t i=0;i<10000;++i)
        {
            word_t  x = ran.fuzz<uint32_t>();
            natural X = x;
            word_t  y = ran.fuzz<uint32_t>();
            natural Y = y;

            word_t  s = x+y;
            natural S = X+Y;
            if( S.to_word() != s )
            {
                throw exception("addition failure");
            }

        }
    }

    // substraction
    {
        for(size_t i=0;i<1000;++i)
        {
            word_t  x = ran.full<word_t>();
            natural X = x;
            word_t  y = ran.full<word_t>();
            natural Y = y;

            if(y>x)
            {
                cswap(x,y);
                X.xch(Y);
            }
            assert(x>=y);
            assert(X>=Y);
            natural s = X-Y;
            if(x-y!=s.to_word()) throw exception("sub failure");
        }

    }

}
YOCTO_UNIT_TEST_DONE()
