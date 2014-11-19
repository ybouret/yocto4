#include "yocto/math/types.hpp"

#define real_t float
#include "types.cxx"

#undef real_t
#define real_t double
#include "types.cxx"

namespace yocto
{
    namespace math
    {
        template <> const float            numeric<float>::zero  = 0.0f;
        template <> const double           numeric<double>::zero = 0.0;
        template <> const complex<float>   numeric< complex<float> >::zero(0.0f,0.0f);
        template <> const complex<double>  numeric< complex<double> >::zero(0.0,0.0);
        template <> const ptrdiff_t        numeric<long>::zero = 0;
        
        template <> const float            numeric<float>::one  = 1.0f;
        template <> const double           numeric<double>::one = 1.0;
        template <> const complex<float>   numeric< complex<float> >::one(1.0f,0.0f);
        template <> const complex<double>  numeric< complex<double> >::one(1.0,0.0);
        template <> const ptrdiff_t        numeric<long>::one = 1;
        
        template <> const float            numeric<float>::two  = 2.0f;
        template <> const double           numeric<double>::two = 2.0;
        template <> const complex<float>   numeric< complex<float> >::two(2.0f,0.0f);
        template <> const complex<double>  numeric< complex<double> >::two(2.0,0.0);
        template <> const ptrdiff_t        numeric<long>::two = 2;

    }
    
}
