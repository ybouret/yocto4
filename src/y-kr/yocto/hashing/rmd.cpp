#include "yocto/hashing/rmd.hpp"
#include <cstdio>
#include <iostream>

namespace yocto
{
    
    namespace hashing
    {
        rmd:: ~rmd() throw()
        {
        }
        
        rmd:: rmd() throw() :
        length(0),
        nx(0),
        nb(0),
        X(),
        B()
        {
        }
        
        
        void rmd:: reset() throw()
        {
            (size_t&)length = nx = nb = 0;
        }
        
#define B2X() ( ( uint32_t(B[3]) << 24 ) |  ( uint32_t(B[2]) << 16 ) |  ( uint32_t(B[1]) << 8 ) | uint32_t(B[0]) )
        bool rmd:: store( uint8_t b ) throw()
        {
            assert(nb<4);
            assert(nx<16);
            ++((size_t&)length);
            B[nb++] = b;
            if(nb>=4)
            {
                X[nx++] = B2X();
                //std::cerr.flush();
                //fprintf(stderr,"[rmd]: +%08x (length=%u)\n", X[nx-1], unsigned(length));
                //fflush(stderr);
                nb = 0;
                if( nx >= 16 )
                {
                    //std::cerr.flush();
                    //fprintf(stderr,"[rmd]: <BLOCK>\n" );
                    //fflush(stderr);
                    nx = 0;
                    return true;
                }
            }
            return false;
        }
        
        const uint32_t * rmd:: block() const throw()
        {
            assert(0==nx);
            assert(0==nb);
            assert(length>0);
            return X;
        }
        
        const uint8_t * rmd:: flush() throw()
        {
            assert(nx<16);
            assert(nb<4);
            for(size_t i=nb;i<4;++i)
                B[i] = 0;
            X[nx] = B2X();
            return (uint8_t *)X;
        }
        
        uint32_t rmd:: lswlen() const throw()
        {
            return uint32_t(length&0xFFFFFFFFUL);
        }
        
        uint32_t rmd:: mswlen() const throw()
        {
            return uint32_t( (length>>32) & 0xFFFFFFFFUL);
        }
        
    }
}
