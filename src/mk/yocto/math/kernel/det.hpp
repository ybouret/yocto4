#ifndef YOCTO_DET_INCLUDED
#define YOCTO_DET_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {
        
        //! M is destroyed on ouput
        template <typename T>
        inline T __determinant_of( matrix<T> &M ) throw()
        {
            assert(M.is_square());
            assert(M.rows>0);
            
            T s(1);
            const size_t n  = M.rows;
            for(size_t i=1,i1=i+1;i<=n;++i,++i1)
            {
                //______________________________________________________________
                //
                // find max pivot
                // if i==n, just check no singular matrix
                //______________________________________________________________
                T       piv = Fabs(M[i][i]);
                size_t  ipv = i;
                for(size_t k=i1;k<=n;++k)
                {
                    const T tmp = Fabs(M[k][i]);
                    if( tmp > piv )
                    {
                        piv = tmp;
                        ipv = k;
                    }
                }
                if(piv<=0)
                    return 0; // singular matrix
                
                //______________________________________________________________
                //
                // set the pivot row
                //______________________________________________________________
                if(ipv!=i)
                {
                    s=-s;
                    M.swap_rows(ipv,i);
                }
                const T pivot(M[i][i]);
                for(size_t k=i1;k<=n;++k)
                {
                    const T coef = M[k][i];
                    M[k][i] = 0;
                    for(size_t j=i1;j<=n;++j)
                    {
                        M[k][j] -= (M[i][j]*coef)/pivot;
                    }
                }
            }
            
            //__________________________________________________________________
            //
            // Compute determinant
            //__________________________________________________________________
            T d(1);
            for(size_t i=n;i>0;--i)
            {
                d *= M[i][i];
            }
            
            return s*d;
        }
        
        //! M is not destroyed on ouput
        template <typename T>
        inline T determinant_of( const matrix<T> &M )
        {
            matrix<T> MM(M);
            return __determinant_of<T>(MM);
        }
        
        template <typename T>
        inline T cofactor_of( const matrix<T> &M, size_t I, size_t J, matrix<T> &m )
        {
            assert(M.is_square());
            assert(M.rows>=1);
            assert(m.rows==M.rows-1);
            if(M.rows>1)
            {
                m.minor_of(M,I,J);
                const T det = __determinant_of(m);
                return ( ((I+J)&1) != 0 ) ? -det : det;
            }
            else
            {
                return M[1][1];
            }
        }
        
        template <typename T>
        inline void adjoint( matrix<T> &A, const matrix<T> &M )
        {
            assert(M.is_square());
            assert(A.is_square());
            assert(M.rows==A.rows);
            switch(M.rows)
            {
                case 0:
                    return;
                    
                case 1:
                    A[1][1] = T(1);
                    return;
                    
                default: {
                    const size_t n  = M.rows;
                    const size_t n1 = n-1;
                    matrix<T>    m(n1,n1);
                    for(size_t i=n;i>0;--i)
                    {
                        for(size_t j=n;j>0;--j)
                        {
                            A[i][j] = cofactor_of(M, j, i, m);
                        }
                    }
                    
                }
                    
            }
            
        }
        
        
    }
}

#endif
