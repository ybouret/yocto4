#ifndef YOCTO_CODE_PRIMES_DIFF_INCLUDED
#define YOCTO_CODE_PRIMES_DIFF_INCLUDED

#include "yocto/os.hpp"

namespace yocto
{
    namespace core
    {
        struct Primes
        {
            static const size_t  DIFF_SIZE = 32768;
            static const uint8_t DIFF_BYTE[DIFF_SIZE];
            static void  Print();
            static const size_t  NEXT = 386129;
        };
    }
    
    bool   isPrime( size_t n );
    size_t nextPrime(size_t n);
    
}

#endif
