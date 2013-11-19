#ifndef YOCTO_MATH_FFT_BITREV_INCLUDED
#define YOCTO_MATH_FFT_BITREV_INCLUDED 1

#include "yocto/code/bswap.hpp"

namespace yocto
{
 
    namespace math
    {
        
        //! bit reversal for SIZE, data[2*SIZE]
        template <typename T,size_t size>
        inline void bitrev( T *data ) throw()
        {
            assert(data);
            static const size_t n=size<<1;
            size_t j=1;
            for (size_t i=1;i<n;i+=2)
            {
                if(j>i)
                {
                    core::bswap<2*sizeof(real_t)>( data+i, data+j);
                }
                size_t m = size; // m=  n / 2;
                while( m>=2 && j>m )
                {
                    j -= m;
                    m >>= 1;
                }
                j += m;
            }
        }
        
    }
    
}

#endif
