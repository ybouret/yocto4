#ifndef YOCTO_MATH_CORE_DET_INCLUDED
#define YOCTO_MATH_CORE_DET_INCLUDED 1

#include "yocto/container/matrix.hpp"

namespace yocto
{
    namespace math
    {
        //! compute determinant, destructive, for float, double, complex<float/double>, mpq
        template <typename T>
        inline T __determinant( matrix<T> &a )
        {
            assert(a.rows==a.cols);
            assert(a.rows>0);
            typedef typename matrix<T>::scalar_type scalar_t;
            typedef typename matrix<T>::row         row_t;

            const scalar_t amin = xnumeric<scalar_t>::minimum();
            bool           s    = false;
            const size_t   n    = a.rows;
            T              ans  = xnumeric<T>::one();

            for(size_t i=1;i<n;++i)
            {
                //______________________________________________________________
                //
                // find max pivot
                //______________________________________________________________
                row_t       &a_i   = a[i];
                T           &pivot = a[i][i];
                size_t       ii    = i;
                scalar_t     piv   = Fabs(a_i[i]);
                for(size_t k=n;k>i;--k)
                {
                    const scalar_t tmp = Fabs(a[k][i]);
                    if(tmp>piv)
                    {
                        piv = tmp;
                        ii  = k;
                    }
                }

                if(piv<=amin)
                {
                    return xnumeric<T>::zero(); // singular matrix...
                }

                if(ii>i)
                {
                    s=!s;
                    a.swap_rows(ii,i);
                }

                // value of referenced pivot is updated: update det
                ans *= pivot;

                //______________________________________________________________
                //
                // triangularize, not set to zero...
                //______________________________________________________________
                for(size_t k=n;k>i;--k)
                {
                    row_t   &a_k   = a[k];
                    const T &coeff = a_k[i];
                    for(size_t j=n;j>i;--j)
                    {
                        T &a_kj = a_k[j];
                        a_kj = (a_kj*pivot - a_i[j]*coeff)/pivot;
                    }
                }

            }
            ans *= a[n][n];
            return s ? -ans : ans;
        }



        //! non destructive determinant
        template <typename T>
        inline T determinant( const matrix<T> &a )
        {
            matrix<T> aa(a,YOCTO_MATRIX_TIGHTEN);
            return __determinant(aa);
        }

        //! determinant for signed integer types, or float/double
        template <typename T>
        inline T ideterminant(const matrix<T> &a )
        {
            matrix<double> aa(a.rows,a.cols);
            for(size_t i=a.rows;i>0;--i)
            {
                const typename matrix<T>::row &src = a[i];
                matrix<double>::row           &tgt = aa[i];
                for(size_t j=a.cols;j>0;--j)
                {
                    tgt[j] = src[j];
                }
            }
            return T( floor(__determinant(aa)+0.5) );
        }

        

    }
}

#endif
