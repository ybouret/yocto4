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
                lw_array<size_t>   indx( a.__indices(), n);
                lw_array<scalar_t> scal( a.__scalars(), n);
                if(dneg) *dneg = false;

                //______________________________________________________________
                //
                // initializing scaling/pivot
                //______________________________________________________________
                const scalar_t amin = xnumeric<scalar_t>::minimum();
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
                    if( piv <= amin )
                    {
                        return false;
                    }
                    scal[i] = __one/piv;
                }


                //------------------------------------------------------------------
                // Crout's algorithm
                //------------------------------------------------------------------
                for(size_t j=1;j<=n;++j)
                {
                    for(size_t i=1;i<j;++i)
                    {
                        row_t &a_i = a[i];
                        T      sum = a_i[j];
                        for(size_t k=1;k<i;++k)
                        {
                            sum -= a_i[k]*a[k][j];
                        }
                        a_i[j]=sum;
                    }

                    scalar_t piv  = __zero;
                    size_t   imax = j;
                    for( size_t i=j;i<=n;i++)
                    {
                        row_t &a_i = a[i];
                        T      sum = a_i[j];
                        for(size_t k=1;k<j;++k)
                        {
                            sum -= a_i[k]*a[k][j];
                        }
                        a_i[j]=sum;

                        const scalar_t tmp = scal[i]*Fabs(sum);
                        if( tmp >= piv )
                        {
                            piv  = tmp;
                            imax = i;
                        }
                    }

                    //-- TODO: check ?
                    //assert( piv > 0 );
                    assert( imax> 0 );
                    if (j != imax)
                    {
                        a.swap_rows( j, imax );
                        if(dneg)
                        {*dneg = ! *dneg;
                        }
                        scal[imax]=scal[j];
                    }

                    indx[j]=imax;

                    if( Fabs(a[j][j]) <= amin )
                    {
                        //std::cerr << "-- LU failure level-2" << std::endl;
                        return false;
                    }

                    if (j != n)
                    {
                        const T fac = __one/(a[j][j]);
                        for(size_t i=j+1;i<=n;++i)
                        {
                            a[i][j] *= fac;
                        }
                    }
                }

#if !defined(NDEBUG)
                for( size_t i=1; i <= n; ++i )
                {
                    assert( indx[i] >  0 );
                    assert( indx[i] <= n );
                }
#endif
                return true;
                
            }
            
        };
        
    }
}

#endif
