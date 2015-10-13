#ifndef YOCTO_MATH_CHOLESKY_INCLUDED
#define YOCTO_MATH_CHOLESKY_INCLUDED 1

#include "yocto/math/matrix.hpp"
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

            //! a.is_square() and diag.size() >= a.rows
            /**
             only the upper part of a is required, and
             the result is stored in the lower triangle,
             but the diagonal into diag.
             */
            static inline
            bool build( matrix_t &a, array_t &diag ) throw()
            {
                assert(a.rows==a.cols);
                assert(diag.size()>=a.rows);
                const T      __zero(0);
                const size_t n = a.rows;
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
                return true;
            }

            //! build the matrix L, may be the same as a. a MUST be large
            static inline
            bool build(matrix_t &L, const matrix_t &a) throw()
            {
                assert(L.rows==a.rows);
                assert(L.cols==a.cols);
                assert(L.rows==L.cols);
                assert(a.memory_kind == matrix_large_memory);
                const T      __zero(0);
                const size_t n = a.rows;
                lw_array<T>  d( a.get_aux(0), n);
                L.assign(a);
                if(!build(L,d))
                {
                    return false;
                }
                else
                {
                    for(size_t i=1;i<=n;++i)
                    {
                        L[i][i] = d[i];
                        for(size_t j=i+1;j<=n;++j)
                        {
                            L[i][j] = __zero;
                        }
                    }
                    return true;
                }
            }
            

        };

    }
}

#endif
