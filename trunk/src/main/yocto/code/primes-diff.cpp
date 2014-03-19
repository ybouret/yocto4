#include "yocto/code/primes-diff.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace core
    {
        const uint8_t Primes::DIFF_BYTE[ Primes::DIFF_SIZE ] =
        {
#include "primes-diff.cxx"
        };
        
        void Primes:: Print()
        {
            std::cout << 2 << std::endl;
            std::cout << 3 << std::endl;
            size_t  p = 3;
            for(size_t i=0;i< DIFF_SIZE;++i)
            {
                p += DIFF_BYTE[i];
                std::cout << p << std::endl;
                if(p>20)
                    break;
            }
            std::cout << NEXT << std::endl;
        }
        
    }
    
    static inline bool isPrimeGT5( const size_t n )
    {
        //______________________________________________________________________
        //
        // use sieve
        //______________________________________________________________________
        size_t q = 3;
        for(size_t i=0;i<core::Primes::DIFF_SIZE;++i)
        {
            q += core::Primes::DIFF_BYTE[i];
            if( q*q > n )
                return true;
            if( 0 == ( n % q ) )
                return false;
        }
        
        //______________________________________________________________________
        //
        // then probe
        //______________________________________________________________________
        q  = core::Primes::NEXT;
        while( q * q <= n )
		{
			if( 0 == (n % q)  ) return false;
			q += 2;
		}
		return true;
        
    }
    
    bool isPrime(  size_t n )
    {
        if( n<=1 || (0==(n%2)) || (0==(n%3) ) )
            return false;
        else
            return isPrimeGT5(n);
    }

    size_t nextPrime(size_t n)
    {
       if(n<=2)
           return 2;
        size_t q = n;
        if(!(q&1)) ++q; assert((q&1)!=0);
        while( !isPrime(q) )
            q+=2;
        
        if(q<n)
            throw exception("nextPrime overflow");
        return q;
    }
    
    
}
