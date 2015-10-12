#ifndef YOCTO_MATH_CORE_ADJOINT_INCLUDED
#define YOCTO_MATH_CORE_ADJOINT_INCLUDED 1

#include "yocto/math/core/determinant.hpp"

namespace yocto
{
    namespace math
    {
        template <typename T >
        inline T cofactor(const matrix<T> &M,
                          const size_t     I,
                          const size_t     J,
                          matrix<T>       &m)
        {
            assert(M.is_square());
            assert(m.is_square());
            assert(M.rows>=1);
            assert(m.rows==M.rows-1);
            if(M.rows>1)
            {
                m.minor_of(M,I,J);
                const T ddet = __determinant(m);
                const T mdet = ( ((I+J)&1) != 0 ) ? -ddet : ddet;
                return mdet;
            }
            else
            {
                return M[1][1];
            }
        }


        template <typename T>
        inline void adjoint(matrix<T> &A, const matrix<T> &M)
        {
            assert(A.rows==M.rows);
            assert(A.cols==M.cols);
            assert(M.is_square());
            const size_t n = M.rows;
            switch(n)
            {
                case 0:
                    return;

                case 1:
                    A[1][1] = M[1][1];
                    return;

                default:
                    break;
            }
            assert(n>=2);
            matrix<T> m(n-1,n-1);
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    A[j][i] = cofactor(M,i,j,m);
                }
            }
        }

        //! for signed integer type
        template <typename T >
        inline T icofactor(const matrix<T> &M,
                           const size_t     I,
                           const size_t     J,
                           matrix<double>  &m)
        {

            assert(M.is_square());
            assert(m.is_square());
            assert(M.rows>=1);
            assert(m.rows==M.rows-1);
            if(M.rows>1)
            {
                m.minor_of<T>(M,I,J);
                const T ddet = T(floor(__determinant(m)+0.5));
                const T mdet = ( ((I+J)&1) != 0 ) ? -ddet : ddet;
                return mdet;
            }
            else
            {
                return M[1][1];
            }
        }

        //! adjoint of integer matrices
        template <typename T>
        inline void iadjoint(matrix<T> &A, const matrix<T> &M)
        {
            assert(A.rows==M.rows);
            assert(A.cols==M.cols);
            assert(M.is_square());
            const size_t n = M.rows;
            switch(n)
            {
                case 0:
                    return;

                case 1:
                    A[1][1] = M[1][1];
                    return;

                default:
                    break;
            }
            assert(n>=2);
            matrix<double> m(n-1,n-1);
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    A[j][i] = icofactor(M,i,j,m);
                }
            }
        }



    }
}


#endif
