
#include "yocto/math/fft/fftn.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    
    namespace math
    {
        
        void FFT( matrix< complex<float> > &M ) throw()
        {
            assert(is_a_power_of_two(M.rows));
            assert(is_a_power_of_two(M.cols));
            const size_t nn[4] = { 0, M.rows, M.cols, 0 };
            FourierN<float>::Forward( (&M[1][1].re)-1, nn, 2);
        }
        
        void iFFT( matrix< complex<float> > &M ) throw()
        {
            assert(is_a_power_of_two(M.rows));
            assert(is_a_power_of_two(M.cols));
            const size_t nn[4] = { 0, M.rows, M.cols, 0 };
            FourierN<float>::Reverse( (&M[1][1].re)-1, nn, 2);
        }

        void FFT( matrix< complex<double> > &M ) throw()
        {
            assert(is_a_power_of_two(M.rows));
            assert(is_a_power_of_two(M.cols));
            const size_t nn[4] = { 0, M.rows, M.cols, 0 };
            FourierN<double>::Forward( (&M[1][1].re)-1, nn, 2);
        }
        
        void iFFT( matrix< complex<double> > &M ) throw()
        {
            assert(is_a_power_of_two(M.rows));
            assert(is_a_power_of_two(M.cols));
            const size_t nn[4] = { 0, M.rows, M.cols, 0 };
            FourierN<double>::Reverse( (&M[1][1].re)-1, nn, 2);
        }

    }

}

