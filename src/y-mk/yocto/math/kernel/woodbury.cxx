#include "yocto/math/kernel/woodbury.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace math
    {
        
        static inline
        void __finalize( matrix<z_type> &B )
        {
            assert(B.is_square());
            const size_t n = B.rows;
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>i;--j)
                {
                    B[i][j] = -B[i][j];
                }
                B[i][i] = numeric<z_type>::one - B[i][i];
                for(size_t j=i-1;j>0;--j)
                {
                    B[i][j] = -B[i][j];
                }
            }
            
        }
        
        template <>
        bool woodbury<z_type>:: build(matrix<z_type>       &B,
                                      const lu<z_type>     &LU,
                                      const matrix<z_type> &A,
                                      const matrix<z_type> &U,
                                      const matrix<z_type> &V)
        {
            assert(A.rows>0);
            assert(A.cols==A.rows);
            assert(U.rows == A.rows );
            assert(V.rows == A.rows );
            assert(V.cols == U.cols);
            assert(U.cols>0);
            
            const size_t n = A.rows;
            const size_t p = U.cols;
            
            matrix<z_type> Z(U);
            matrix<z_type> H(p,p);
            lu<z_type>     L(p);
            
            LU.solve(A,Z);
            algebra<z_type>::mul_ltrn(H, V, Z);
            for(size_t i=p;i>0;--i) H[i][i] += numeric<z_type>::one;
            if( !L.build(H) )
            {
                // singular H matrix !!!
                return false;
            }
            
            matrix<z_type> tV(V,matrix_transpose);
            L.solve(H, tV);
            B.make(n,n);
            algebra<z_type>::mul(B, Z, tV);
            __finalize(B);
            return true;
        }
        
    }
    
    
}
