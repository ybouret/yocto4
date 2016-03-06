#ifndef YOCTO_MATH_FFTN_INCLUDED
#define YOCTO_MATH_FFTN_INCLUDED 1

#include "yocto/math/complex.hpp"
#include "yocto/container/matrix.hpp"


namespace yocto
{
    
    namespace math
    {

        template <typename T>
        struct FourierN
        {
            //! generic fourier
            /**
             nn[1]..nn[ndim]
             data[1..prod(nn)*2], row major array of 'complexes'
             */
            static void Compute(T *data, const size_t nn[], const size_t ndim, int isign) throw();
            
            static inline void Forward( T *data, const size_t nn[], const size_t ndim) throw()
            {
                Compute(data, nn, ndim, 1);
            }
            
            static inline void Reverse( T *data, const size_t nn[], const size_t ndim) throw()
            {
                Compute(data, nn, ndim, -1);
            }
            
        };
        
        void FFT( matrix< complex<float>  > &) throw();
        void iFFT(matrix< complex<float>  > &) throw();
        void FFT( matrix< complex<double> > &) throw();
        void iFFT(matrix< complex<double> > &) throw();
        
    }

}

#endif
