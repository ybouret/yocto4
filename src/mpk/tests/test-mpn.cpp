#include "yocto/utest/run.hpp"
#include "yocto/mpk/word2mpn.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/comparator.hpp"
#include "yocto/random/bits.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/sort/quick.hpp"

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
    std::cerr << "-- Addition Tests" << std::endl;
    {
        for(natural i=0;i<100;++i,i++)
        {
            (void)i;
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
    std::cerr << "-- Subtraction Tests" << std::endl;
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
        
        for(natural n = 100; n>0; --n)
        {
            (void)n;
        }
        
        vector<natural> nn;
        for(size_t i=0;i<300;++i)
        {
            const natural tmp = ran.full<word_t>();
            nn.push_back(tmp);
        }
        quicksort(nn);
        c_shuffle(nn(),nn.size());
        natural seed = ran.full<word_t>();
        natural sorg = seed;
        _SHOW(seed);
        for(size_t i=1;i<=nn.size();++i)
        {
            seed += nn[i];
        }
        _SHOW(seed);_SHOW_BITS(seed);
        c_shuffle(nn(),nn.size());
        for(size_t i=1;i<=nn.size();++i)
        {
            seed -= nn[i];
        }
        _SHOW(seed);
        if(seed!=sorg)
        {
            throw exception("mismatch...");
        }
    }
    
    std::cerr << "-- Multiplication Tests" << std::endl;
    {
        for(size_t i=0;i<10000;++i)
        {
            word_t  x = ran.fuzz<uint32_t>();
            natural X = x;
            word_t  y = ran.fuzz<uint32_t>();
            natural Y = y;
            
            word_t  p = x*y;
            natural P = X*Y;
            if(p!=P.to_word())
            {
                throw exception("multiplication error");
            }
            
            X = ran.full<word_t>();
            Y = natural::sqr(X);
            P = X*X;
            if(P!=Y)
                throw exception("Square Error");
        }
        
        for(natural n=0;n<=10;++n)
        {
            const natural f = natural::factorial(n);
            std::cerr << n.to_word() << "! = " << f << std::endl;
        }
        
    }
    
    std::cerr << "-- Bits Tests" << std::endl;
    {
        natural X;
        for(size_t n=0;n<=30;++n)
        {
            X = natural::exp2(n);
            std::cerr << "2^" << n << " = " << X << std::endl;
            while( X > 0 )
            {
                std::cerr << "\t" << X.shr() << std::endl;
            }
            X = natural::exp2(n);
            while(X>0)
            {
                X = natural::shr(X,1+alea_leq(4));
                std::cerr << "\t\t" << X << std::endl;
            }
        }
        for(size_t n=0;n<=100;++n)
        {
            for(size_t i=0;i<10;++i)
            {
                natural x = natural::rand(n);
                if(x.bits()!=n)
                    throw exception("invalid #bits");
            }
        }
    }
    
    std::cerr << "-- Division Tests" << std::endl;
    {
        for(size_t i=0;i<1000;++i)
        {
            word_t  x = ran.full<word_t>();
            natural X = x;
            word_t  y = 1+ran.full<uint32_t>();
            natural Y = y;
            
            const word_t  d = x/y;
            const natural D = X/Y;
            if( d != D )
            {
                throw exception("division error");
            }
        }
        
        vector<natural> nn;
        for(size_t i=0;i<10;++i)
        {
            const natural tmp = 1+ran.full<word_t>();
            nn.push_back(tmp);
        }
        natural seed = ran.full<word_t>();
        natural sorg = seed;
        for(size_t i=1;i<=nn.size();++i) seed *= nn[i];
        c_shuffle(nn(), nn.size());
        for(size_t i=1;i<=nn.size();++i) seed /= nn[i];
        if(seed!=sorg)
            throw exception("division error, level2");
    }
    
    std::cerr << "-- Modulo Tests" << std::endl;
    {
        for(size_t i=0;i<1000;++i)
        {
            word_t  x = ran.full<word_t>();
            natural X = x;
            word_t  y = ran.full<word_t>();
            if(y<=0) y =1;
            natural Y = y;
            
            const word_t  r = x%y;
            const natural R = X%Y;
            if( R != r )
            {
                throw exception("modulo error");
            }
            
            const natural Q = X/Y;
            
            natural Q2,R2;
            natural::split(Q2, R2, X, Y);
            if(R2!=R)
            {
                throw exception("split error");
            }
            
        }
    }
    
    std::cerr << "-- Arithmetic Tests" << std::endl;
    {
        natural x = 100;
        natural y = 5;
        natural g = natural::gcd(x,y);
        std::cerr << "gcd=" << g << std::endl;
        
        for(size_t i=0;i<10;++i)
        {
            natural b = natural::rand(40);
            natural e = natural::rand(7);
            natural n = natural::rand(20);
            natural p = natural::mod_exp(b,e,n);
            std::cerr << b << "^" << e << "[" << n << "]=" << p << std::endl;
            natural q = natural::power(b,e);
            q = q%n;
            std::cerr << "q=" << q << std::endl;
            if(q!=p) throw exception("mod_exp failure");
        }
    }
    
}

YOCTO_UNIT_TEST_DONE()

#include "yocto/sys/timings.hpp"
#include "yocto/ios/ocstream.hpp"

YOCTO_UNIT_TEST_IMPL(mul)
{
    const size_t    nval = 32;
    vector<natural> values(nval,as_capacity);
    timings         tmx;
    ios::ocstream::overwrite("mul.dat");
    for(size_t nbits=8;nbits<=512;nbits*=2)
    {
        std::cerr << "nbits=" << nbits << std::endl;
        values.free();
        for(size_t i=nval;i>0;--i)
        {
            const natural tmp = natural::rand(nbits);
            values.push_back(tmp);
        }
        const size_t n  = values.size();
        const size_t n1 = n+1;
        natural p;
        YOCTO_TIMINGS(tmx, 1, for(size_t i=n;i>0;--i) { p = values[i] * values[n1-i]; } (void)p );
        tmx.speed *= n;
        std::cerr << "speed=" << tmx.speed << std::endl;
        ios::acstream fp("mul.dat");
        fp("%g %g\n", double(nbits), tmx.speed);
    }
    
}
YOCTO_UNIT_TEST_DONE()

