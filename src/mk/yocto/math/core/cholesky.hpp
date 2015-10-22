#ifndef YOCTO_MATH_CHOLESKY_INCLUDED
#define YOCTO_MATH_CHOLESKY_INCLUDED 1

#include "yocto/container/matrix.hpp"
#include "yocto/sequence/lw-array.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        struct cholesky
        {

            typedef matrix<T> matrix_t;
            typedef array<T>  array_t;

            //! a.is_square()
            /**
             only the upper part of a is required, and
             the result is stored in the lower triangle,
             but the diagonal into diag.
             a MUST have some extra memory
             */
            static inline
            bool build( matrix_t &a ) throw()
            {
                assert(a.rows==a.cols);
                assert(a.memory_kind == matrix_large_memory);
                const T      __zero(0);
                const size_t n = a.rows;
                lw_array<T>  diag( a.get_aux(0), n);
                for(size_t i=1;i<=n;++i)
                {
                    typename matrix_t::row &a_i = a[i];
                    for(size_t j=i;j<=n;++j )
                    {
                        typename matrix_t::row &a_j = a[j];
                        T sum = a_i[j];
                        for( size_t k=i-1;k>0;--k)
                            sum -= a_i[k] * a_j[k];

                        if( i == j )
                        {
                            if( sum <= __zero )
                                return false;
                            diag[i] = Sqrt( sum );
                        }
                        else
                        {
                            a_j[i] = sum / diag[i];
                        }
                    }
                }
                for(size_t i=n;i>0;--i)
                {
                    a[i][i] = diag[i];
                    for(size_t j=n;j>i;--j)
                    {
                        a[i][j] = 0;
                    }
                }
                return true;
            }

            //! solve a*x=b, a was decomposed...
            static inline
            void solve( array_t &x, const matrix_t &a, const array_t &b) throw()
            {
                assert(a.rows==a.cols);
                assert(x.size()>=a.rows);
                assert(b.size()>=a.rows);
                const size_t n = a.rows;

                for( size_t i=1; i<=n; ++i )
                {
                    T sum = b[i];
                    for( size_t k=i-1; k>0;--k )
                        sum -= a[i][k] * x[k];
                    x[i] = sum / a[i][i];
                }
                
                for( size_t i=n; i>0; --i )
                {
                    T sum = x[i];
                    for( size_t k=i+1; k <=n; ++k )
                        sum -= a[k][i] * x[k];
                    x[i] = sum / a[i][i];
                }

            }

        };
        
    }
}

#endif
