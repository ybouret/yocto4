#ifndef YOCTO_MATH_CORE_LU_INCLUDED
#define YOCTO_MATH_CORE_LU_INCLUDED 1


#include "yocto/math/core/matrix.hpp"
#include "yocto/sequence/lw-array.hpp"

namespace yocto
{
    namespace math
    {

        //! work for: float,double, complex<float>, complex<double>, mpq
        template <typename T>
        struct LU
        {
            typedef YOCTO_MATRIX<T>           matrix_t;
            typedef typename matrix_t::row    row_t;
            typedef typename real_of<T>::type scalar_t;
            static inline
            bool build( matrix_t &a, bool *dneg=0)
            {
                assert(a.cols>0);
                assert(a.rows>0);
                assert(a.is_square());
                const scalar_t  __zero = xnumeric<scalar_t>::zero();
                const scalar_t  __one  = xnumeric<scalar_t>::one();
                const size_t    n = a.rows;
                lw_array<size_t> indx( a.__indices(), n);
                lw_array<T>      scal( a.__aux(0), n );
                if(dneg) *dneg = false;

                //______________________________________________________________
                //
                // initializing scaling/pivot
                //______________________________________________________________
                for( size_t i=n;i>0;--i)
                {
                    const row_t & a_i = a[i];
                    scalar_t      piv = __zero;

                    for( size_t j=n;j>0;--j)
                    {
                        const scalar_t tmp = Fabs( a_i[j] );
                        if (tmp>piv)
                        {
                            piv = tmp;
                        }
                    }
                    if( false /*piv <= numeric<scalar_t>::minimum*/ )
                    {
                        return false;
                    }
                    scal[i] = __one/piv;
                }

                
                return false;
            }

        };

    }
}

#endif
