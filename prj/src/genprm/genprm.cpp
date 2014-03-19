#include "yocto/ios/ocstream.hpp"
#include "yocto/exception.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/string/conv.hpp"

#include <iostream>

using namespace yocto;

typedef uint64_t word_t;

static inline bool __is_prime( word_t n )
{
    if(n<=1)
        return false;
    if(0 == n%2)
        return false;
    if(0 == n%3)
        return false;
    
    word_t q = 5;
    
    while(true)
    {
        if(q*q>n)
            return true;
        if(0==n%q)
            return false;
        q += 2;
    }
}

static inline word_t __next_prime( word_t n )
{
    while( ! __is_prime(++n) )
        ;
    return n;
}


int main(int argc, char *argv[])
{
    try
    {
        if(argc<=1)
            throw exception("need max_count");
        const size_t max_count = strconv::to<size_t>( argv[1], "max_count");
        size_t count = 0;
        // 2 and 3 are primes
        word_t n    = 3;
        word_t dmax = 0;
        vector<unsigned> dif;
        while(true)
        {
            const word_t q = __next_prime(n);
            ++count;
            const word_t delta = q - n;
            if( delta&1 ) throw exception("odd delta!");
            std::cerr << q << " / " << delta << std::endl;
            if(delta>dmax)
                dmax = delta;
            if(dmax > 255 )
            {
                --count;
                break;
            }
            n=q;
            dif.push_back(delta);
            if(count>=max_count)
            {
                break;
            }
        }
        std::cerr << "#prm=" << count << std::endl;
        assert(count==dif.size());
        std::cerr << "dmax=" << dmax << std::endl;
        ios::ocstream fp("prm.dat",false);
        for(size_t i=1;i<count;++i)
        {
            fp("0x%02x, ", dif[i]);
            if( 0 == (i%16) )
                fp("\n");
        }
        fp("0x%02x\n", dif[count]);
        
        const word_t nxt = __next_prime(n);
        std::cerr << "next = " << nxt << std::endl;
        return 0;
    }
    catch(const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unhandled error" << std::endl;
    }
    return 1;
}